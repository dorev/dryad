#pragma once

#include <stdio.h>
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <map>

namespace Dryad
{

    //
    // Logging
    //

    enum class LogLevel
    {
        Debug = 0,
        Info,
        Warning,
        Error,
        Fatal
    };

    inline static LogLevel gLogLevel = LogLevel::Debug;

    #ifdef _MSC_VER
    #define DRYAD_FUNCTION __FUNCSIG__
    #else
    #define DRYAD_FUNCTION __PRETTY_FUNCTION__
    #endif

    #define DRYAD_UNUSED(variable) (void) variable

    #define DRYAD_COLOR_RESET       "\033[0m"
    #define DRYAD_COLOR_GREEN       "\033[32m"
    #define DRYAD_COLOR_WHITE       "\033[37m"
    #define DRYAD_COLOR_YELLOW      "\033[33m"
    #define DRYAD_COLOR_RED         "\033[31m"
    #define DRYAD_COLOR_BRIGHT_RED  "\033[91m"

    #define DRYAD_LOG(level, color, tag, format, ...) \
        if (gLogLevel <= LogLevel::level) \
        { \
            printf(color "[" tag "] " format " (%s)\n" DRYAD_COLOR_RESET, ##__VA_ARGS__, DRYAD_FUNCTION); \
        }

    #define DRYAD_DEBUG(format, ...) DRYAD_LOG(Debug,   DRYAD_COLOR_GREEN,      "DEBUG",    format, ##__VA_ARGS__)
    #define DRYAD_INFO(format, ...)  DRYAD_LOG(Info,    DRYAD_COLOR_WHITE,      "INFO",     format, ##__VA_ARGS__)
    #define DRYAD_WARN(format, ...)  DRYAD_LOG(Warning, DRYAD_COLOR_YELLOW,     "WARNING",  format, ##__VA_ARGS__)
    #define DRYAD_ERROR(format, ...) DRYAD_LOG(Error,   DRYAD_COLOR_RED,        "ERROR",    format, ##__VA_ARGS__)
    #define DRYAD_FATAL(format, ...) DRYAD_LOG(Fatal,   DRYAD_COLOR_BRIGHT_RED, "FATAL",    format, ##__VA_ARGS__)

    //
    // Allocation
    //

    #define DRYAD_NEW(type, ...) new type(__VA_ARGS__)
    #define DRYAD_DELETE(pointer) if (pointer) { delete pointer; pointer = nullptr; }

    template<class T>
    using SharedPtr = std::shared_ptr<T>;

    template<class To, class From>
    SharedPtr<To> SharedPtrCast(const SharedPtr<From>& sharedPointer)
    {
        return std::static_pointer_cast<To>(sharedPointer);
    }

    //
    // Containers
    //

    using String = std::string;

    template <class T>
    using Vector = std::vector<T>;

    template <class T, class compare = std::less<T>>
    using Set = std::set<T, compare>;

    template <class key, class value>
    using Map = std::map<key, value>;

} // namespace Dryad
