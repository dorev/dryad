#pragma once

#include "includes.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOG(x) std::cout << x << "\n"
#define CRASH(x) { std::cout << "\n\n" << x << "\n --> " << __FILE__ << " l." << __LINE__ << "\n\n"; \
                   *(reinterpret_cast<unsigned int*>(0xBAADD00DBAADD00D)) = 0U; throw; }

//#define PROFILING_ENABLED

#if defined(PROFILING_ENABLED)
    #define PROFILE(x) x;
#else
    #define PROFILE(x)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Utility macro

#define for_range(index_variable, limit) for(int index_variable = 0; index_variable < static_cast<int>(limit); ++index_variable)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
