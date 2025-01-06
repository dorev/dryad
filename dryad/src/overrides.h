#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <set>
#include <map>

//
// LOGGING
//

enum class dryad_log_level
{
    debug = 0,
    info,
    warning,
    error,
    fatal
};

inline static dryad_log_level g_log_level = dryad_log_level::debug;

#ifdef _MSC_VER
#define DRYAD_FUNCTION __FUNCSIG__
#else
#define DRYAD_FUNCTION __PRETTY_FUNCTION__
#endif

#define DRYAD_COLOR_RESET       "\033[0m"
#define DRYAD_COLOR_GREEN       "\033[32m"
#define DRYAD_COLOR_WHITE       "\033[37m"
#define DRYAD_COLOR_YELLOW      "\033[33m"
#define DRYAD_COLOR_RED         "\033[31m"
#define DRYAD_COLOR_BRIGHT_RED  "\033[91m"

#define DRYAD_LOG(level, color, tag, format, ...) \
    if (g_log_level <= dryad_log_level::level) \
    { \
        printf(color "[" tag "] " format " (%s)\n" DRYAD_COLOR_RESET, ##__VA_ARGS__, DRYAD_FUNCTION); \
    }

#define DRYAD_DEBUG(format, ...) DRYAD_LOG(debug,   DRYAD_COLOR_GREEN,      "DEBUG",    format, ##__VA_ARGS__)
#define DRYAD_INFO(format, ...)  DRYAD_LOG(info,    DRYAD_COLOR_WHITE,      "INFO",     format, ##__VA_ARGS__)
#define DRYAD_WARN(format, ...)  DRYAD_LOG(warning, DRYAD_COLOR_YELLOW,     "WARNING",  format, ##__VA_ARGS__)
#define DRYAD_ERROR(format, ...) DRYAD_LOG(error,   DRYAD_COLOR_RED,        "ERROR",    format, ##__VA_ARGS__)
#define DRYAD_FATAL(format, ...) DRYAD_LOG(fatal,   DRYAD_COLOR_BRIGHT_RED, "FATAL",    format, ##__VA_ARGS__)

//
// ALLOCATION
//

#define DRYAD_NEW(type, ...) new type(##__VA_ARGS__)
#define DRYAD_DELETE(pointer) if (pointer) { delete pointer; pointer = nullptr; }

//
// CONTAINERS
//

using dryad_string = std::string;

template <class T>
using dryad_vector = std::vector<T>;

template <class T, class compare = std::less<T>>
using dryad_set = std::set<T, compare>;

template <class key, class value>
using dryad_map = std::map<key, value>;
