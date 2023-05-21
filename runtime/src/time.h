#pragma once

#include "types.h"
#include <limits>

namespace Dryad 
{
    using Time = float;
    using Tempo = float;
    const Tempo DefaultTempo = 108;

    struct Fraction
    {
        Fraction(UInt32 numerator = 0, UInt32 denominator = 0)
            : numerator(numerator)
            , denominator(denominator)
        {
        }

        bool IsValid() const
        {
            return denominator > 0;
        }

        float AsFloat() const
        {
            return IsValid()
                ? static_cast<float>(numerator) / static_cast<float>(denominator)
                : std::numeric_limits<float>::max();
        }

        bool operator>(float value) const
        {
            return AsFloat() > value;
        }

        UInt32 numerator;
        UInt32 denominator;
    };

    struct ScoreTime : Fraction
    {
        ScoreTime(UInt32 numerator = 0, UInt32 denominator = 0, Time time = 0)
            : Fraction(numerator, denominator)
            , time(time)
        {
        }

        Time time;
    };

    using TimeSignature = Fraction;
}