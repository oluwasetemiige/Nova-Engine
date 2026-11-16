#include "Nova/Core/Log.h"

#include <atomic>
#include <cstdio>
#include <deque>
#include <mutex>

namespace Nova
{
    namespace
    {
        struct LogState
        {
            std::mutex Mutex;
            std::deque<LogEntry> Entries;
            std::size_t MaxEntries = 1000;
            bool ConsoleOutput = true;
            std::atomic<std::uint64_t> Revision{0};
        };

        LogState& State()
        {
            static LogState state; // constructed on first use, safe during static init
            return state;
        }

        const char* LevelTag(LogLevel level)
        {
            switch (level)
            {
            case LogLevel::Trace: return "TRACE";
            case LogLevel::Info: return "INFO";
            case LogLevel::Warn: return "WARN";
            case LogLevel::Error: return "ERROR";
            }
            return "?";
        }
    } // namespace

    void Log::Write(LogLevel level, std::string message)
    {
        LogState& s = State();
        std::lock_guard lock(s.Mutex);

        if (s.ConsoleOutput)
        {
            std::FILE* out = (level == LogLevel::Error) ? stderr : stdout;
            std::fprintf(out, "[%s] %s\n", LevelTag(level), message.c_str());
            std::fflush(out);
        }

        s.Entries.push_back({level, std::move(message)});
        while (s.Entries.size() > s.MaxEntries)
            s.Entries.pop_front();

        ++s.Revision;
    }

    std::vector<LogEntry> Log::Entries()
    {
        LogState& s = State();
        std::lock_guard lock(s.Mutex);
        return {s.Entries.begin(), s.Entries.end()};
    }

    std::uint64_t Log::Revision()
    {
        return State().Revision.load();
    }

    void Log::Clear()
    {
        LogState& s = State();
        std::lock_guard lock(s.Mutex);
        s.Entries.clear();
        ++s.Revision;
    }

    void Log::SetMaxEntries(std::size_t maxEntries)
    {
        LogState& s = State();
        std::lock_guard lock(s.Mutex);
        s.MaxEntries = maxEntries == 0 ? 1 : maxEntries;
        while (s.Entries.size() > s.MaxEntries)
            s.Entries.pop_front();
    }

    void Log::SetConsoleOutput(bool enabled)
    {
        LogState& s = State();
        std::lock_guard lock(s.Mutex);
        s.ConsoleOutput = enabled;
    }
} // namespace Nova
