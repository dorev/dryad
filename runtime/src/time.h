#pragma once

#include "types.h"

namespace Dryad 
{
    using Tempo = float;
    using Time = float;

    struct Fraction
    {
        UInt32 numerator;
        UInt32 denominator;
    };

    struct MusicTime : Fraction
    {
        Time time;
        Tempo tempo;
        bool withinTempoChange;
        Fraction tempoChangeProgress;
    };

    using Duration = MusicTime;
    using Position = MusicTime;
    using TimeSignature = Fraction;

    constexpr MusicTime TimeZero { 0, 0, 0.0f, 0.0f, false, {0, 0} };
}