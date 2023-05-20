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

    enum class ChordQualityFlags
    {
        Minor = 0,
        Major = 1,
        Diminished = 2,
        Augmented = 3,
        Sus2 = 4,
        Sus4 = 5,
        DiminishedSeventh = 6,
        HalfDiminished = 7,
        FLAG(Seventh, 4),
        FLAG(MajorSeventh, 5),
        FLAG(Add6, 6),
        FLAG(Add9, 7),
        FLAG(Add11, 8),
        FLAG(Add13, 9),
        FLAG(Flat2, 10),
        FLAG(Flat3, 11),
        FLAG(Flat4, 12),
        FLAG(Flat5, 13),
        FLAG(Flat6, 14),
        FLAG(Flat7, 15),
        FLAG(Flat9, 16),
        FLAG(Flat11, 17),
        FLAG(Flat13, 18),
        FLAG(Sharp2, 19),
        FLAG(Sharp3, 20),
        FLAG(Sharp4, 21),
        FLAG(Sharp5, 22),
        FLAG(Sharp6, 23),
        FLAG(Sharp7, 24),
        FLAG(Sharp9, 25),
        FLAG(Sharp11, 26),
        FLAG(Sharp13, 27),
        //FLAG(PLACEHOLDER_BIT0, 28),
        //FLAG(PLACEHOLDER_BIT1, 29),
        //FLAG(PLACEHOLDER_BIT2, 30),
        //FLAG(PLACEHOLDER_BIT3, 31),
    };

    enum class DegreeFlags
    {
        FLAG(Tonic, 1),
        FLAG(Supertonic, 2),
        FLAG(Mediant, 3),
        FLAG(Subdominant, 4),
        FLAG(Dominant, 5),
        FLAG(Submediant, 6),
        FLAG(LeadingTone, 7),
    };

    struct NoteRange
    {
        NoteValue min;
        NoteValue max;
        DegreeFlags allowedDegrees;
    };

    struct Chord
    {
        NoteValue rootOffset;
        DegreeFlags degree;
        Accidental accidental;
        ChordQualityFlags qualities;
    };
}
