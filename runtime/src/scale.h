#pragma once

#include "types.h"
#include "chord.h"
#include "constants.h"

namespace Dryad
{
    struct ScaleOffsets
    {
        ScaleOffsets
        (
            NoteValue tonic = 0,
            NoteValue supertonic = 2,
            NoteValue mediant = 4,
            NoteValue subdominant = 5,
            NoteValue dominant = 7,
            NoteValue submediant = 9,
            NoteValue leadindTone = 11
        )
            : offsets
            {
                tonic,
                supertonic,
                mediant,
                subdominant,
                dominant,
                submediant,
                leadindTone
            }
        {
        }

        NoteValue offsets[7];
    };

    const ScaleOffsets MajorScaleOffsets = ScaleOffsets();
    const ScaleOffsets MinorNaturalOffsets = ScaleOffsets(0, 2, 3, 5, 7, 8, 10);
    const ScaleOffsets MinorHarmonicOffsets = ScaleOffsets(0, 2, 3, 5, 7, 8, 11);
    const ScaleOffsets MinorMelodicOffsets = ScaleOffsets(0, 2, 3, 5, 7, 9, 11);

    struct ScaleDegrees
    {
        ScaleDegrees
        (
            Chord first = Chord(Degree::Tonic, ChordQualities::Major | ChordQualities::MajorSeventh),
            Chord second = Chord(Degree::Supertonic, ChordQualities::Minor | ChordQualities::Seventh),
            Chord third = Chord(Degree::Mediant, ChordQualities::Minor | ChordQualities::Seventh),
            Chord fourth = Chord(Degree::Subdominant, ChordQualities::Major | ChordQualities::MajorSeventh),
            Chord fifth = Chord(Degree::Dominant, ChordQualities::Major | ChordQualities::MajorSeventh),
            Chord sixth = Chord(Degree::Submediant, ChordQualities::Major | ChordQualities::Seventh),
            Chord seventh = Chord(Degree::LeadingTone, ChordQualities::HalfDiminished)
        )
            : degrees
            {
                first,
                second,
                third,
                fourth,
                fifth,
                sixth,
                seventh
            }
        {
        }

        Chord degrees[7];
    };

    class Scale
    {
    public:
        Scale
        (
            ScaleOffsets offsets = ScaleOffsets(),
            ScaleOffsets descendingOffsets = ScaleOffsets(),
            ScaleDegrees degrees = ScaleDegrees(),
            NoteValue root = C
        )
            : offsets(offsets)
            , descendingOffsets(descendingOffsets)
            , degrees(degrees)
            , root(root)
        {
        }

        ScaleOffsets offsets;
        ScaleOffsets descendingOffsets;
        ScaleDegrees degrees;
        NoteValue root;
    };
}
