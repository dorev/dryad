#pragma once

#include <stdio.h>

namespace Dryad
{
    enum class LogLevel
    {
        Debug = 0,
        Info,
        Warning,
        Error,
        Fatal
    };

    inline static LogLevel g_LogLevel = LogLevel::Debug;
}

#ifdef _MSC_VER
    #define FUNCTION_SIGNATURE __FUNCSIG__
#else
    #define FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#endif

#define LOG_DEBUG(format, ...) do { if (g_LogLevel <= LogLevel::Debug) { printf("[DEBUG] " format " (%s)\n", ##__VA_ARGS__, FUNCTION_SIGNATURE); } } while(0)
#define LOG_INFO(format, ...) do { if (g_LogLevel <= LogLevel::Info) { printf("[INFO] " format " (%s)\n", ##__VA_ARGS__, FUNCTION_SIGNATURE); } } while(0)
#define LOG_WARN(format, ...) do { if (g_LogLevel <= LogLevel::Warning) { printf("[WARNING] " format " (%s)\n", ##__VA_ARGS__, FUNCTION_SIGNATURE); } } while(0)
#define LOG_ERROR(format, ...) do { if (g_LogLevel <= LogLevel::Error) { printf("[ERROR] " format " (%s)\n", ##__VA_ARGS__, FUNCTION_SIGNATURE); } } while(0)
#define LOG_FATAL(format, ...) do { if (g_LogLevel <= LogLevel::Fatal) { printf("[FATAL] " format " (%s)\n", ##__VA_ARGS__, FUNCTION_SIGNATURE); } } while(0)





