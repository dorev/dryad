#pragma once

#include "types.h"

namespace Dryad
{
    template <class T, size_t Size = sizeof(T)>
    struct UnsignedOfSameSize;

    template <class T>
    struct UnsignedOfSameSize<T, 1>
    {
        using Type = unsigned char;
    };

    template <class T>
    struct UnsignedOfSameSize<T, 2>
    {
        using Type = unsigned short;
    };

    template <class T>
    struct UnsignedOfSameSize<T, 4>
    {
        using Type = unsigned int;
    };

    template <class T>
    struct UnsignedOfSameSize<T, 8>
    {
        using Type = unsigned long long;
    };

    template <class FlagType>
    void SetFlag(FlagType& target, FlagType flagToSet)
    {
        using CastType = UnsignedOfSameSize<FlagType>::Type;
        CastType result = static_cast<CastType>(target) | static_cast<CastType>(flagToSet);
        target = static_cast<FlagType>(result);
    }
    
    template <class FlagType>
    void ClearFlag(FlagType& target, FlagType flagToClear)
    {
        using CastType = UnsignedOfSameSize<FlagType>::Type;
        CastType result = static_cast<CastType>(target) & ~(static_cast<CastType>(flagToClear));
        target = static_cast<FlagType>(result);
    }

    template <class FlagType>
    bool FlagIsSet(const FlagType& target, FlagType flagValue)
    {
        using CastType = UnsignedOfSameSize<FlagType>::Type;
        return (static_cast<CastType>(target) & (static_cast<CastType>(flagValue))) > 0;
    }
}