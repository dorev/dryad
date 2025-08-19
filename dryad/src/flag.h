#pragma once

#include "types.h"

#define DRYAD_FLAG(name, shift) name = 1 << shift

#define DRYAD_DECLARE_FLAG_ENUM(EnumName, EnumType) \
    enum class EnumName : EnumType; \
    constexpr EnumName operator|(EnumName a, EnumName b) \
    { \
        return static_cast<EnumName>(static_cast<EnumType>(a) | static_cast<EnumType>(b)); \
    } \
    constexpr EnumName operator&(EnumName a, EnumName b) \
    { \
        return static_cast<EnumName>(static_cast<EnumType>(a) & static_cast<EnumType>(b)); \
    } \
    inline EnumName& operator|=(EnumName& a, EnumName b) \
    { \
        return a = a | b; \
    } \
    inline EnumName& operator&=(EnumName& a, EnumName b) \
    { \
        return a = a & b; \
    } \
    enum class EnumName : EnumType


