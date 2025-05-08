#pragma once
#include "Log.h"

namespace Athena
{
#if defined(_MSC_VER)
    #define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__)
    #include <signal.h>
    #define DEBUG_BREAK() raise(SIGABRT)
#else
    #define DEBUG_BREAK() ((void)0)
#endif
    

#ifdef ATH_ENABLE_ASSERTS
    #define ATH_ASSERT(x, ...) { if(!(x)) { LOG_FATAL("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK(); } }
#else
    #define ATH_ASSERT(x, ...)
#endif
}
