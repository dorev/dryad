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

#define DRYAD_COLOR_RESET "\033[0m"
#define DRYAD_COLOR_GREEN "\033[32m"
#define DRYAD_COLOR_WHITE "\033[37m"
#define DRYAD_COLOR_YELLOW "\033[33m"
#define DRYAD_COLOR_RED "\033[31m"
#define DRYAD_COLOR_BRIGHT_RED "\033[91m"

#define DRYAD_LOG(level, color, tag, format, ...) \
    if (g_LogLevel <= LogLevel::level) \
    { \
        printf(color "[" tag "] " format " (%s)\n" DRYAD_COLOR_RESET, ##__VA_ARGS__, DRYAD_FUNCTION); \
    }

#define DRYAD_DEBUG(format, ...) DRYAD_LOG(Debug, DRYAD_COLOR_GREEN, "DEBUG", format, ##__VA_ARGS__)
#define DRYAD_INFO(format, ...)  DRYAD_LOG(Info, DRYAD_COLOR_WHITE, "INFO", format, ##__VA_ARGS__)
#define DRYAD_WARN(format, ...)  DRYAD_LOG(Warning, DRYAD_COLOR_YELLOW, "WARNING", format, ##__VA_ARGS__)
#define DRYAD_ERROR(format, ...) DRYAD_LOG(Error, DRYAD_COLOR_RED, "ERROR", format, ##__VA_ARGS__)
#define DRYAD_FATAL(format, ...) DRYAD_LOG(Fatal, DRYAD_COLOR_BRIGHT_RED, "FATAL", format, ##__VA_ARGS__)



