#pragma once
#include <fmt/os.h>
#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/ranges.h>

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

        static void Init(const std::string& filename)
        {
            //auto out = fmt::output_file(filename);
            //_ostream = fmt::output_file(filename);

        }

        static void Shutdown()
        {

        }

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

            fmt::print(fmt::fg(levelColor), "[{0}] [{1}]: ", GetTimestamp(), levelStr);
            fmt::print(fmt::fg(levelColor), args...);
            fmt::print("\n");
        }

    private:
        static std::string GetTimestamp()
        {
            auto now = std::chrono::system_clock::now();
            auto inTime = std::chrono::system_clock::to_time_t(now);
            char buffer[100];
            std::strftime(buffer, sizeof(buffer), "%H:%M:%S", std::localtime(&inTime));
            return std::string(buffer);
        }

    private:
        static fmt::v11::ostream _ostream;
    };
}

#if DEBUG
    #define LOG_INFO(...) ::Athena::Logger::Log(::Athena::Logger::LogLevel::Info, __VA_ARGS__)
    #define LOG_DEBUG(...) ::Athena::Logger::Log(::Athena::Logger::LogLevel::Debug, __VA_ARGS__)
    #define LOG_WARN(...) ::Athena::Logger::Log(::Athena::Logger::LogLevel::Warning, __VA_ARGS__)
    #define LOG_ERROR(...) ::Athena::Logger::Log(::Athena::Logger::LogLevel::Error, __VA_ARGS__)
    #define LOG_FATAL(...) ::Athena::Logger::Log(::Athena::Logger::LogLevel::Fatal, __VA_ARGS__)
#else
    #define LOG_DEBUG(...)
    #define LOG_INFO(...)
    #define LOG_WARN(...)
    #define LOG_ERROR(...)
    #define LOG_FATAL(...)
#endif
