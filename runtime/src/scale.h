#pragma once

#include "types.h"
#include "chord.h"

namespace Dryad
{
    constexpr ScaleOffsets MajorOffsets = { 0, 2, 4, 5, 7, 9, 11 };
    constexpr ScaleOffsets MinorNaturalOffsets = { 0, 2, 3, 5, 7, 8, 10 };
    constexpr ScaleOffsets MinorHarmonicOffsets = { 0, 2, 3, 5, 7, 8, 11 };
    constexpr ScaleOffsets MinorMelodicOffsets = { 0, 2, 3, 5, 7, 9, 11 };

    class Scale
    {
        ScaleOffsets offsets;
        ScaleOffsets descendingOffsets;
        Chord degrees[7];
        NoteValue root;
    };

    using ScalePtr = SharedPtr<Scale>;
}
