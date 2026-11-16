#pragma once

#include <cstddef>
#include <cstdint>
#include <format>
#include <string>
#include <utility>
#include <vector>

namespace Nova
{
    enum class LogLevel
    {
        Trace,
        Info,
        Warn,
        Error
    };

    struct LogEntry
    {
        LogLevel Level = LogLevel::Info;
        std::string Message;
    };

    // Thread-safe logger. Every message goes to the terminal and into a bounded
    // in-memory history that the editor's Console panel reads.
    class Log
    {
    public:
        template <typename... Args>
        static void Trace(std::format_string<Args...> fmt, Args&&... args)
        {
            Write(LogLevel::Trace, std::format(fmt, std::forward<Args>(args)...));
        }

        template <typename... Args>
        static void Info(std::format_string<Args...> fmt, Args&&... args)
        {
            Write(LogLevel::Info, std::format(fmt, std::forward<Args>(args)...));
        }

        template <typename... Args>
        static void Warn(std::format_string<Args...> fmt, Args&&... args)
        {
            Write(LogLevel::Warn, std::format(fmt, std::forward<Args>(args)...));
        }

        template <typename... Args>
        static void Error(std::format_string<Args...> fmt, Args&&... args)
        {
            Write(LogLevel::Error, std::format(fmt, std::forward<Args>(args)...));
        }

        static void Write(LogLevel level, std::string message);

        // Copy of the retained history, oldest first.
        static std::vector<LogEntry> Entries();

        // Increases by one on every Write() and Clear(); lets UIs detect changes cheaply.
        static std::uint64_t Revision();

        static void Clear();
        static void SetMaxEntries(std::size_t maxEntries);
        static void SetConsoleOutput(bool enabled);
    };
} // namespace Nova
