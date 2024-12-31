#pragma once

#include "types.h"

namespace dryad
{
    constexpr dryad_bpm DefaultTempo = 120.0f;

    constexpr dryad_time Whole = 96;
    constexpr dryad_time HalfDotted = 72;
    constexpr dryad_time Half = 48;
    constexpr dryad_time QuarterDotted = 36;
    constexpr dryad_time HalfTriplet = 32;
    constexpr dryad_time Quarter = 24;
    constexpr dryad_time EighthDotted = 18;
    constexpr dryad_time QuarterTriplet = 16;
    constexpr dryad_time Eighth = 12;
    constexpr dryad_time EighthTriplet = 8;
    constexpr dryad_time Sixteenth = 6;
    constexpr dryad_time SixteenthTriplet = 4;
    constexpr dryad_time ThirthySeconth = 3;
    constexpr dryad_time ThirthySeconthTriplet = 2;
    constexpr dryad_time DefaultBeatAlignment = Eighth;

    dryad_time nearest_beat_before(dryad_time beatValue, dryad_time scoreTime);
    dryad_time nearest_beat_after(dryad_time beatValue, dryad_time scoreTime);
}
