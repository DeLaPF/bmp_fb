#include "logging.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace BFGe {
    Log::Log(std::string name)
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        this->m_logger = spdlog::stdout_color_mt(name);
        this->m_logger->set_level(spdlog::level::trace);
    }

    Log Logger::s_coreLogger("BFGE");
    Log Logger::s_clientLogger("APP");
}
