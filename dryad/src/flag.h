#pragma once

#include "types.h"

#define DRYAD_FLAG(name, shift) name = 1 << shift

#define DRYAD_DECLARE_FLAG_ENUM(EnumName, enum_type) \
    enum class EnumName : enum_type; \
    constexpr EnumName operator|(EnumName a, EnumName b) \
    { \
        return static_cast<EnumName>(static_cast<enum_type>(a) | static_cast<enum_type>(b)); \
    } \
    constexpr EnumName operator&(EnumName a, EnumName b) \
    { \
        return static_cast<EnumName>(static_cast<enum_type>(a) & static_cast<enum_type>(b)); \
    } \
    inline EnumName& operator|=(EnumName& a, EnumName b) \
    { \
        return a = a | b; \
    } \
    inline EnumName& operator&=(EnumName& a, EnumName b) \
    { \
        return a = a & b; \
    } \
    enum class EnumName : enum_type


