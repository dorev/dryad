#pragma once

#include "types.h"

namespace dryad
{
    constexpr dryad_bpm DefaultTempo = 120.0f;

    constexpr dryad_score_time Whole = 96;
    constexpr dryad_score_time HalfDotted = 72;
    constexpr dryad_score_time Half = 48;
    constexpr dryad_score_time QuarterDotted = 36;
    constexpr dryad_score_time HalfTriplet = 32;
    constexpr dryad_score_time Quarter = 24;
    constexpr dryad_score_time EighthDotted = 18;
    constexpr dryad_score_time QuarterTriplet = 16;
    constexpr dryad_score_time Eighth = 12;
    constexpr dryad_score_time EighthTriplet = 8;
    constexpr dryad_score_time Sixteenth = 6;
    constexpr dryad_score_time SixteenthTriplet = 4;
    constexpr dryad_score_time ThirthySeconth = 3;
    constexpr dryad_score_time ThirthySeconthTriplet = 2;
    constexpr dryad_score_time DefaultBeatAlignment = Eighth;

    dryad_score_time nearest_beat_before(dryad_score_time beatValue, dryad_score_time scoreTime);
    dryad_score_time nearest_beat_after(dryad_score_time beatValue, dryad_score_time scoreTime);
}
