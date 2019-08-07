#include "Log.h"

#include "Triangle.h"
#include "VertexData.h"

#include "RDMFile.h"

struct Timer {

    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<double>                      duration;

    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }
    ~Timer()
    {
        end      = std::chrono::high_resolution_clock::now();
        duration = end - start;
        spdlog::debug("Execution took: {} ms",duration.count() * 1000.0);
    }
};

int main()
{
    Timer tick;
    Log::initSPDLOG("rdm-converter.log");

	for (auto& p : std::filesystem::recursive_directory_iterator(std::filesystem::path("test\\"))) {

        if (p.path().extension() == ".rdm") {
            std::filesystem::path testPath(p.path());
            RDMFile               testRDM(testPath);
            testRDM.toOBJFile(testPath.replace_extension(".obj"));
        }
    }


    //spdlog::trace("TRACE");
    //spdlog::debug("DEBUG");
    //spdlog::info("info");
    //spdlog::warn("warn");
    //spdlog::error("error");
    //spdlog::critical("critical");
}
