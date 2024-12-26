#pragma once

#include "types.h"

#define DRYAD_FLAG(name, shift) name = 1 << shift

#define DRYAD_DECLARE_FLAG_ENUM(enum_name, enum_type) \
    enum class enum_name : enum_type; \
    constexpr enum_name operator|(enum_name a, enum_name b) \
    { \
        return static_cast<enum_name>(static_cast<enum_type>(a) | static_cast<enum_type>(b)); \
    } \
    constexpr enum_name operator&(enum_name a, enum_name b) \
    { \
        return static_cast<enum_name>(static_cast<enum_type>(a) & static_cast<enum_type>(b)); \
    } \
    inline enum_name& operator|=(enum_name& a, enum_name b) \
    { \
        return a = a | b; \
    } \
    inline enum_name& operator&=(enum_name& a, enum_name b) \
    { \
        return a = a & b; \
    } \
    enum class enum_name : enum_type


