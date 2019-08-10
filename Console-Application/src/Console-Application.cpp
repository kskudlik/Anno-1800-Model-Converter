#include "RDM-Converter.h"
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

        spdlog::debug("Execution took: {} us ({} ms)", duration, ms);
    }

  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimepoint;
};
int main(int argc, char* argv)
{

    Timer tick;
    Log::initSPDLOG("rdm-converter.log");

    HWND  consoleWnd = GetConsoleWindow();
    DWORD dwProcessId;
    GetWindowThreadProcessId(consoleWnd, &dwProcessId);
    if (GetCurrentProcessId() == dwProcessId) {
        spdlog::critical("YOU ARE DOING IT WRONG!");
        system("pause");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

    //spdlog::trace("TRACE");
    //spdlog::debug("DEBUG");
    //spdlog::info("info");
    //spdlog::warn("warn");
    //spdlog::error("error");
    //spdlog::critical("critical");
}
