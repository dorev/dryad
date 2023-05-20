#pragma once

#include "types.h"

namespace Dryad 
{
    using Time = float;
    using Tempo = float;
    const Tempo DefaultTempo = 108;

    struct Fraction
    {
        Fraction()
            : numerator(0)
            , denominator(0)
        {
        }

        Fraction(UInt32 numerator, UInt32 denominator)
            : numerator(numerator)
            , denominator(denominator)
        {
        }

        UInt32 numerator;
        UInt32 denominator;
    };

    struct MusicTime : Fraction
    {
        MusicTime()
            : time(0)
            , tempo(DefaultTempo)
            , withinTempoChange(false)
            , tempoChangeProgress()
        {
        }

        Time time;
        Tempo tempo;
        bool withinTempoChange;
        Fraction tempoChangeProgress;
    };

    using Duration = MusicTime;
    using Position = MusicTime;
    using TimeSignature = Fraction;
}