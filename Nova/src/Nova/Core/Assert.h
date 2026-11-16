#pragma once

#include "Nova/Core/Log.h"

#include <cstdlib>

#if defined(_MSC_VER)
    #include <intrin.h>
    #define NOVA_DEBUGBREAK() __debugbreak()
#elif !defined(NDEBUG)
    #include <csignal>
    #define NOVA_DEBUGBREAK() std::raise(SIGTRAP)
#else
    #define NOVA_DEBUGBREAK() std::abort()
#endif

// Always-on assertion for programmer errors. `message` should be a string literal.
#define NOVA_ASSERT(condition, message)                                                     \
    do                                                                                      \
    {                                                                                       \
        if (!(condition))                                                                   \
        {                                                                                   \
            ::Nova::Log::Error("Assertion failed: {} -- {} ({}:{})", #condition, message,   \
                               __FILE__, __LINE__);                                         \
            NOVA_DEBUGBREAK();                                                              \
            std::abort();                                                                   \
        }                                                                                   \
    } while (false)
