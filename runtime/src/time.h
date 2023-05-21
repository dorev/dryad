#pragma once

#include "types.h"

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

        UInt32 numerator;
        UInt32 denominator;
    };

    struct MusicTime : Fraction
    {
        MusicTime(UInt32 numerator = 0, UInt32 denominator = 0, Time time = 0, Tempo = DefaultTempo)
            : Fraction(numerator, denominator)
            , time(time)
            , tempo(tempo)
        {
        }

        Time time;
        Tempo tempo;
    };

    using Duration = MusicTime;
    using Position = MusicTime;
    using TimeSignature = Fraction;
}