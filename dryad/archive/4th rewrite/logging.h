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
    #define DRYAD_FUNCTION __FUNCSIG__
#else
    #define DRYAD_FUNCTION __PRETTY_FUNCTION__
#endif

#define DRYAD_DEBUG(format, ...) do { if (g_LogLevel <= LogLevel::Debug) { printf("[DEBUG] " format " (%s)\n", ##__VA_ARGS__, DRYAD_FUNCTION); } } while(0)
#define DRYAD_INFO(format, ...) do { if (g_LogLevel <= LogLevel::Info) { printf("[INFO] " format " (%s)\n", ##__VA_ARGS__, DRYAD_FUNCTION); } } while(0)
#define DRYAD_WARN(format, ...) do { if (g_LogLevel <= LogLevel::Warning) { printf("[WARNING] " format " (%s)\n", ##__VA_ARGS__, DRYAD_FUNCTION); } } while(0)
#define DRYAD_ERROR(format, ...) do { if (g_LogLevel <= LogLevel::Error) { printf("[ERROR] " format " (%s)\n", ##__VA_ARGS__, DRYAD_FUNCTION); } } while(0)
#define DRYAD_FATAL(format, ...) do { if (g_LogLevel <= LogLevel::Fatal) { printf("[FATAL] " format " (%s)\n", ##__VA_ARGS__, DRYAD_FUNCTION); } } while(0)





