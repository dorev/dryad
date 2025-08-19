#pragma once

#include "types.h"
#include "flag.h"

namespace Dryad
{

    enum class Degree
    {
        Tonic = 0,
        Root = Tonic,
        Supertonic = 1,
        Second = Supertonic,
        Mediant = 2,
        Third = Mediant,
        Subdominant = 3,
        Fourth = Subdominant,
        Dominant = 4,
        Fifth = Dominant,
        Submediant = 5,
        Sixth = Submediant,
        LeadingTone = 7,
        Seventh = LeadingTone,
        Limit = 8, 
        Invalid = -1
    };

    DRYAD_DECLARE_FLAG_ENUM(ChordQuality, int)
    {
        None = 0,
        Minor = 1,
        Major = 2,
        HalfDiminished = 3,
        Augmented = 4,
        Sus2 = 5,
        Sus4 = 6,
        Diminished = 7,
        DRYAD_FLAG(SeventhMinor, 4),
        DRYAD_FLAG(SeventhMajor, 5),
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
        //DRYAD_FLAG(PLACEHOLDER_BIT0, 28),
        //DRYAD_FLAG(PLACEHOLDER_BIT1, 29),
        //DRYAD_FLAG(PLACEHOLDER_BIT2, 30),
        //DRYAD_FLAG(PLACEHOLDER_BIT3, 31),
        Invalid = -1
    };

    enum class Accidental
    {
        Natural = 0,
        Flat = -1,
        Sharp = 1,
    };

    struct Chord
    {
        Chord
        (
            Degree degree = Degree::Invalid,
            ChordQuality qualities = ChordQuality::None,
            Accidental accidental = Accidental::Natural
        )
            : degree(degree)
            , qualities(qualities)
            , accidental(accidental)
        {
        }

        Degree degree;
        ChordQuality qualities;
        Accidental accidental;
    };

} // namespace Dryad
