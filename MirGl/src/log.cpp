#include "log.h"

namespace Mir
{
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::Init()
    {
        // Set the pattern for formatting log messages
        // %^ : start color range
        // %T : ISO 8601 time format (HH:MM:SS)
        // %n : logger name
        // %v : actual message
        // %$ : end color range
        spdlog::set_pattern("%^[%T] %n: %v%$");
        
        // Create loggers with color
        s_CoreLogger = spdlog::stdout_color_mt("MIRGL");
        s_CoreLogger->set_level(spdlog::level::trace);
        
        s_ClientLogger = spdlog::stdout_color_mt("APP");
        s_ClientLogger->set_level(spdlog::level::trace);
    }
}