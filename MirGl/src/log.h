#pragma once

#include <memory>
#include <string>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Mir
{
    class Log
    {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

    // Core log macros
    #define MIR_CORE_TRACE(...)    ::Mir::Log::GetCoreLogger()->trace(__VA_ARGS__)
    #define MIR_CORE_INFO(...)     ::Mir::Log::GetCoreLogger()->info(__VA_ARGS__)
    #define MIR_CORE_WARN(...)     ::Mir::Log::GetCoreLogger()->warn(__VA_ARGS__)
    #define MIR_CORE_ERROR(...)    ::Mir::Log::GetCoreLogger()->error(__VA_ARGS__)
    #define MIR_CORE_CRITICAL(...) ::Mir::Log::GetCoreLogger()->critical(__VA_ARGS__)

    // Client log macros
    #define MIR_TRACE(...)         ::Mir::Log::GetClientLogger()->trace(__VA_ARGS__)
    #define MIR_INFO(...)          ::Mir::Log::GetClientLogger()->info(__VA_ARGS__)
    #define MIR_WARN(...)          ::Mir::Log::GetClientLogger()->warn(__VA_ARGS__)
    #define MIR_ERROR(...)         ::Mir::Log::GetClientLogger()->error(__VA_ARGS__)
    #define MIR_CRITICAL(...)      ::Mir::Log::GetClientLogger()->critical(__VA_ARGS__)
} // namespace Mir     

