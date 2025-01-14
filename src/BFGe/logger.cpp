#include "logger.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace BFGe {
    // TODO: Why do I need to add this here? (if not)
    // ERROR: bmp_fb-entrypoint.lib(entrypoint.obj) : error LNK2001: unresolved external symbol "private: static class std::shared_ptr<class spdlog::logger> BFGe::Logger::s_coreLogger" (?s_coreLogger@Logger@BFGe@@0V?$shared_ptr@Vlogger@spdlog@@@std@@A) [D:\dev\cpp\bmp_fb\builds\windows\examples\audio\audio.vcxproj]
    std::shared_ptr<spdlog::logger> Logger::s_coreLogger;
    std::shared_ptr<spdlog::logger> Logger::s_clientLogger;

    void Logger::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_coreLogger = spdlog::stdout_color_mt("BFGE");
        s_coreLogger->set_level(spdlog::level::trace);

        s_clientLogger = spdlog::stdout_color_mt("APP");
        s_clientLogger->set_level(spdlog::level::trace);
    }
}
