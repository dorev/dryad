#pragma once

#include "types.h"

namespace Dryad
{
    constexpr Tempo DefaultTempo = 120.0f;

    constexpr ScoreTime Whole = 96;
    constexpr ScoreTime HalfDotted = 72;
    constexpr ScoreTime Half = 48;
    constexpr ScoreTime QuarterDotted = 36;
    constexpr ScoreTime HalfTriplet = 32;
    constexpr ScoreTime Quarter = 24;
    constexpr ScoreTime EighthDotted = 18;
    constexpr ScoreTime QuarterTriplet = 16;
    constexpr ScoreTime Eighth = 12;
    constexpr ScoreTime EighthTriplet = 8;
    constexpr ScoreTime Sixteenth = 6;
    constexpr ScoreTime SixteenthTriplet = 4;
    constexpr ScoreTime ThirthySeconth = 3;
    constexpr ScoreTime ThirthySeconthTriplet = 2;
    constexpr ScoreTime DefaultBeatAlignment = Eighth;

    ScoreTime NearestBeatBefore(ScoreTime beatValue, ScoreTime scoreTime);
    ScoreTime NearestBeatAfter(ScoreTime beatValue, ScoreTime scoreTime);
}
