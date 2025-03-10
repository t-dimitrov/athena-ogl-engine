#pragma once
#include <fmt/os.h>
#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>

#include <string>
#include <chrono>
#include <fstream>

namespace Athena
{
    class Logger
    {
    public:
        enum class LogLevel
        {
            Info = 0,
            Debug = 1,
            Warning = 2,
            Error = 3,
            Fatal = 4
        };

        template<typename... Args>
        static void Log(LogLevel level, const Args&... args)
        {
            std::string levelStr;
            fmt::color levelColor{};

            switch (level)
            {
            case LogLevel::Info:
                levelStr = "INFO";
                levelColor = fmt::color::green;
                break;

            case LogLevel::Debug:
                levelStr = "DEBUG";
                levelColor = fmt::color::white;
                break;
                
            case LogLevel::Warning:
                levelStr = "WARNING";
                levelColor = fmt::color::yellow;
                break;

            case LogLevel::Error:
                levelStr = "ERROR";
                levelColor = fmt::color::red;
                break;

            case LogLevel::Fatal:
                levelStr = "FATAL";
                levelColor = fmt::color::purple;
                break;
            }

            auto now = std::chrono::system_clock::now();

            fmt::print(fmt::fg(levelColor), "[{:%H:%M:%S}] [{}]: ", now, levelStr);
            fmt::print(fmt::fg(levelColor), args...);
            fmt::print("\n");
        }
    };

#if DEBUG
    #define LOG_INFO(...) ::Athena::Logger::Log(::Athena::Logger::LogLevel::Info, __VA_ARGS__)
    #define LOG_DEBUG(...) ::Athena::Logger::Log(::Athena::Logger::LogLevel::Debug, __VA_ARGS__)
    #define LOG_WARN(...) ::Athena::Logger::Log(::Athena::Logger::LogLevel::Warning, __VA_ARGS__)
    #define LOG_ERROR(...) ::Athena::Logger::Log(::Athena::Logger::LogLevel::Error, __VA_ARGS__)
    #define LOG_FATAL(...) ::Athena::Logger::Log(::Athena::Logger::LogLevel::Fatal, __VA_ARGS__)
#else
    #define LOG_INFO(...)
    #define LOG_DEBUG(...)
    #define LOG_WARN(...)
    #define LOG_ERROR(...)
    #define LOG_FATAL(...)
#endif

}

