#pragma once
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

class Log
{
  public:
    Log(const char* logfile);
    ~Log();

  private:
    const char* logfile;
};