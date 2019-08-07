#pragma once
#include "pch.h"

namespace Log
{
void initSPDLOG(const char* logfile)
{

    auto console_logger = spdlog::stdout_color_mt("console");
    spdlog::set_default_logger(console_logger);
    spdlog::set_pattern("[%^%l%$] %v");

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logfile, true);
    file_sink->set_pattern("[%Y-%m-%d %T.%e] [%^%l%$] %v");

#if DEBUG == 1
    spdlog::set_level(spdlog::level::trace);
    ;
    file_sink->set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
    ;
    file_sink->set_level(spdlog::level::warn);
#endif

    console_logger->sinks().push_back(file_sink);
}
} // namespace Log
