#pragma once

#include <memory>

#include "spdlog/spdlog.h"

namespace BFGe {
    // static class to enable engine and client logging with macros
    class Logger {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_clientLogger; }
    private:
        static std::shared_ptr<spdlog::logger> s_coreLogger;
        static std::shared_ptr<spdlog::logger> s_clientLogger;
    };
}

// Core log macros
#define BFGE_CORE_TRACE(...)   ::BFGe::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define BFGE_CORE_INFO(...)    ::BFGe::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define BFGE_CORE_WARN(...)    ::BFGe::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define BFGE_CORE_ERROR(...)   ::BFGe::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define BFGE_CORE_FATAL(...)   ::BFGe::Logger::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define BFGE_CLIENT_TRACE(...) ::BFGe::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define BFGE_CLIENT_INFO(...)  ::BFGe::Logger::GetClientLogger()->info(__VA_ARGS__)
#define BFGE_CLIENT_WARN(...)  ::BFGe::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define BFGE_CLIENT_ERROR(...) ::BFGe::Logger::GetClientLogger()->error(__VA_ARGS__)
#define BFGE_CLIENT_FATAL(...) ::BFGe::Logger::GetClientLogger()->fatal(__VA_ARGS__)
