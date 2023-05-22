#pragma once

#include "types.h"
#include "fraction.h"

namespace Dryad 
{
    using Time = float;
    using Tempo = float;
    const Tempo DefaultTempo = 108;

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