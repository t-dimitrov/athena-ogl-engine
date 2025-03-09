#pragma once
#include "Log.h"

#ifdef ATH_ENABLE_ASSERTS
    #define ATH_ASSERT(x, ...) { if(!(x)) { LOG_FATAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define ATH_ASSERT(x, ...)
#endif
