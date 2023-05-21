#pragma once

#include "types.h"

namespace Dryad
{
    enum class Accidental
    {
        Natural,
        Flat,
        Sharp,
    };

    DRYAD_DECLARE_FLAG_ENUM(ChordQualities, UInt32)
    {
        Minor = 0,
        Major = 1,
        Diminished = 2,
        Augmented = 3,
        Sus2 = 4,
        Sus4 = 5,
        DiminishedSeventh = 6,
        HalfDiminished = 7,
        DRYAD_FLAG(Seventh, 4),
        DRYAD_FLAG(MajorSeventh, 5),
        DRYAD_FLAG(Add6, 6),
        DRYAD_FLAG(Add9, 7),
        DRYAD_FLAG(Add11, 8),
        DRYAD_FLAG(Add13, 9),
        DRYAD_FLAG(Flat2, 10),
        DRYAD_FLAG(Flat3, 11),
        DRYAD_FLAG(Flat4, 12),
        DRYAD_FLAG(Flat5, 13),
        DRYAD_FLAG(Flat6, 14),
        DRYAD_FLAG(Flat7, 15),
        DRYAD_FLAG(Flat9, 16),
        DRYAD_FLAG(Flat11, 17),
        DRYAD_FLAG(Flat13, 18),
        DRYAD_FLAG(Sharp2, 19),
        DRYAD_FLAG(Sharp3, 20),
        DRYAD_FLAG(Sharp4, 21),
        DRYAD_FLAG(Sharp5, 22),
        DRYAD_FLAG(Sharp6, 23),
        DRYAD_FLAG(Sharp7, 24),
        DRYAD_FLAG(Sharp9, 25),
        DRYAD_FLAG(Sharp11, 26),
        DRYAD_FLAG(Sharp13, 27),
        //FLAG(PLACEHOLDER_BIT0, 28),
        //FLAG(PLACEHOLDER_BIT1, 29),
        //FLAG(PLACEHOLDER_BIT2, 30),
        //FLAG(PLACEHOLDER_BIT3, 31),
    };

    enum class Degree
    {
        AllDegrees = 0,
        DRYAD_FLAG(Tonic, 1),
        DRYAD_FLAG(Supertonic, 2),
        DRYAD_FLAG(Mediant, 3),
        DRYAD_FLAG(Subdominant, 4),
        DRYAD_FLAG(Dominant, 5),
        DRYAD_FLAG(Submediant, 6),
        DRYAD_FLAG(LeadingTone, 7),
    };

    struct NoteRange
    {
        NoteValue min;
        NoteValue max;
        Degree allowedDegrees;
    };

    struct Chord
    {
        Chord
        (
            Degree degree = Degree::Tonic,
            ChordQualities qualities = ChordQualities::Major,
            Accidental accidental = Accidental::Natural
        )
            : degree(degree)
            , qualities(qualities)
            , accidental(accidental)
        {
        }

        Degree degree;
        ChordQualities qualities;
        Accidental accidental;
    };
}
