#pragma once

#include "types.h"

#define FLAG(name, shift) name = 1 << shift

namespace Dryad
{
    template <class T, size_t Size = sizeof(T)> struct UnsignedOfSameSize;
    template <class T> struct UnsignedOfSameSize<T, 1> { using Type = unsigned char; };
    template <class T> struct UnsignedOfSameSize<T, 2> { using Type = unsigned short; };
    template <class T> struct UnsignedOfSameSize<T, 4> { using Type = unsigned int; };
    template <class T> struct UnsignedOfSameSize<T, 8> { using Type = unsigned long long; };

    //
    // Setting & clearing flags
    //
    template <class FlagType>
    FlagType& SetFlag(FlagType& target, FlagType flagToSet)
    {
        using CastType = UnsignedOfSameSize<FlagType>::Type;
        CastType result = static_cast<CastType>(target) | static_cast<CastType>(flagToSet);
        return target = static_cast<FlagType>(result);
    }
    
    template <class FlagType>
    FlagType& ClearFlag(FlagType& target, FlagType flagToClear)
    {
        using CastType = UnsignedOfSameSize<FlagType>::Type;
        CastType result = static_cast<CastType>(target) & ~(static_cast<CastType>(flagToClear));
        return target = static_cast<FlagType>(result);
    }

    //
    // Reading flags
    //
    template <class FlagType>
    bool FlagIsSet(const FlagType& target, FlagType flagValue)
    {
        using CastType = UnsignedOfSameSize<FlagType>::Type;
        return (static_cast<CastType>(target) & (static_cast<CastType>(flagValue))) > 0;
    }

    template <class FlagType, class... OtherFlags>
    bool AnyFlagIsSet(const FlagType& target, FlagType firstFlag, OtherFlags... otherFlags)
    {
        if (FlagIsSet(target, firstFlag))
        {
            return true;
        }
        return AnyFlagIsSet(target, otherFlags...);
    }

    template <class FlagType>
    bool AnyFlagIsSet(const FlagType& target)
    {
        return false;
    }

    template <class FlagType, class... OtherFlags>
    bool AllFlagsAreSet(const FlagType& target, FlagType firstFlag, OtherFlags... otherFlags)
    {
        if (!FlagIsSet(target, firstFlag))
        {
            return false;
        }
        return AllFlagsAreSet(target, otherFlags...);
    }

    template <class FlagType>
    bool AllFlagsAreSet(const FlagType& target)
    {
        return true;
    }
}
