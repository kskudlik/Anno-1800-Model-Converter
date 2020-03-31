#include "OBJFile.h"
#include "RDM-Converter.h"
#include "Version.h"
#include <shellapi.h>

#include "RDMFile.h"
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

class Timer
{
  public:
    Timer()
    {
        startTimepoint = std::chrono::high_resolution_clock::now();
    }
    ~Timer()
    {
        Stop();
    }
    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint)
                         .time_since_epoch()
                         .count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint)
                       .time_since_epoch()
                       .count();

        auto   duration = end - start;
        double ms       = duration * 0.001;

        spdlog::info("Execution took: {} us ({} ms)", duration, ms);
    }

  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimepoint;
};
static bool inConsole = true;
bool        checkIfInConsole()
{
    HWND  consoleWnd = GetConsoleWindow();
    DWORD dwProcessId;
    GetWindowThreadProcessId(consoleWnd, &dwProcessId);
    if (GetCurrentProcessId() == dwProcessId) {

        inConsole = false;
        return false;
    }
    return true;
}
void printWelcome()
{
    std::cout << "Anno 1800 Model Converter \nVersion: " + std::string(VER_FILE_VERSION_STR)
                     + "\n\n";
}
void printUsage()
{
    //VER_ORIGINAL_FILENAME_STR,
        //                     "Anno 1800 Model Converter \nVersion: " +
        //                     std::string(VER_FILE_VERSION_STR)
        //                         + "\n"
        //                         + "This model converter for the Anno Series - allows you to
        //                         convert "
        //                           "from and into their proprietary .rdm format\n");
    std::cout << "......" << "\n\n";
    spdlog::info("If no output is specified, files will be saved at the same path with the fitting "
                 "extension e.g (.obj or .rdm).\n");
}
void askForGithubIssue()
{

    spdlog::critical("Something went terribly wrong.");
    spdlog::critical("Please create an issue on Github: "
                     "https://github.com/kskudlik/Anno-1800-Model-Converter/issues");
    std::string msg = "Do you want me to take you to the issue page?";

    if (MessageBoxA(NULL, msg.c_str(), VER_FILE_DESCRIPTION_STR,
                    MB_ICONQUESTION | MB_YESNO | MB_SYSTEMMODAL)
        == IDYES) {
        auto result = ShellExecuteA(nullptr, "open",
                                    "https://github.com/kskudlik/Anno-1800-Model-Converter/issues",
                                    nullptr, nullptr, SW_SHOWNORMAL);
        TerminateProcess(GetCurrentProcess(), 0);
    }
}
void convertToOBJ(std::filesystem::path inputPath, std::filesystem::path outputPath)
{
    try {
        RDMFile rdm(inputPath);
        rdm.toOBJFile(outputPath);
    } catch (std::exception e) {
        spdlog::info("Something went wrong...");
        spdlog::error("{}", e.what());
        spdlog::info("When you think this is due to a problem with this converter please create an "
                     "issue on github.");
    } catch (...) {
        askForGithubIssue();
    }
}
void convertToRDM(std::filesystem::path inputPath, std::filesystem::path outputPath)
{
    try {
        OBJFile obj(inputPath);
        RDMFile rdm(obj);
        rdm.toFile(outputPath, VER_FILE_VERSION_STR);
    } catch (std::exception e) {
        spdlog::info("Something went wrong...");
        spdlog::error("{}", e.what());
        spdlog::info("When you think this is due to a problem with this converter please create an "
                     "issue on github.");
    } catch (...) {
        askForGithubIssue();
    }
}

int main(int argc, char** argv)
{

    Log   log("rdm-converter.log");
    Timer tick;

    checkIfInConsole();
    if (argc < 2 || argv[1] == "h" || argv[1] == "help" || argv[1] == "-help" || argv[1] == "-h"
        || argv[1] == "--help" || argv[1] == "--h") {
        printUsage();
        if (!inConsole)
            system("pause");
        if(argc < 2) return EXIT_FAILURE;
        return EXIT_SUCCESS;
    }
 
        std::filesystem::path inputPath  = argv[1];
        std::filesystem::path outputPath = inputPath;
        if (argc > 2) {
            outputPath = argv[2];
        }
        printWelcome();
        if (inputPath.extension() == ".rdm") {
            convertToOBJ(inputPath, outputPath.replace_extension(".obj"));
        } else if (inputPath.extension() == ".obj") {
            convertToRDM(inputPath, outputPath.replace_extension(".rdm"));
        } else {
            spdlog::error("I don't know anything about this {}{} file :(\n",
                          inputPath.filename().string(), inputPath.extension().string());
            printUsage();
            if (!inConsole)
                system("pause");
            return EXIT_FAILURE;
        }

    return EXIT_SUCCESS;
}
