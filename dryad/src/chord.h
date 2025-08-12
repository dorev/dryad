#pragma once

#include "types.h"
#include "flag.h"

namespace Dryad
{

    enum class Degree
    {
        tonic = 0,
        root = tonic,
        supertonic = 1,
        second = supertonic,
        mediant = 2,
        third = mediant,
        subdominant = 3,
        fourth = subdominant,
        dominant = 4,
        fifth = dominant,
        submediant = 5,
        sixth = submediant,
        leadingTone = 7,
        seventh = leadingTone,
        limit = 8, 
        invalid = -1
    };

    DRYAD_DECLARE_FLAG_ENUM(ChordQuality, unsigned)
    {
        none = 0,
        minor = 1,
        major = 2,
        halfDiminished = 3,
        augmented = 4,
        sus2 = 5,
        sus4 = 6,
        diminished = 7,
        DRYAD_FLAG(seventhMinor, 4),
        DRYAD_FLAG(seventhMajor, 5),
        DRYAD_FLAG(add6, 6),
        DRYAD_FLAG(add9, 7),
        DRYAD_FLAG(add11, 8),
        DRYAD_FLAG(add13, 9),
        DRYAD_FLAG(flat2, 10),
        DRYAD_FLAG(flat3, 11),
        DRYAD_FLAG(flat4, 12),
        DRYAD_FLAG(flat5, 13),
        DRYAD_FLAG(flat6, 14),
        DRYAD_FLAG(flat7, 15),
        DRYAD_FLAG(flat9, 16),
        DRYAD_FLAG(flat11, 17),
        DRYAD_FLAG(flat13, 18),
        DRYAD_FLAG(sharp2, 19),
        DRYAD_FLAG(sharp3, 20),
        DRYAD_FLAG(sharp4, 21),
        DRYAD_FLAG(sharp5, 22),
        DRYAD_FLAG(sharp6, 23),
        DRYAD_FLAG(sharp7, 24),
        DRYAD_FLAG(sharp9, 25),
        DRYAD_FLAG(sharp11, 26),
        DRYAD_FLAG(sharp13, 27),
        //DRYAD_FLAG(PLACEHOLDER_BIT0, 28),
        //DRYAD_FLAG(PLACEHOLDER_BIT1, 29),
        //DRYAD_FLAG(PLACEHOLDER_BIT2, 30),
        //DRYAD_FLAG(PLACEHOLDER_BIT3, 31),
        invalid = unsigned(-1)
    };

    enum class Accidental
    {
        natural,
        flat,
        sharp,
    };

    struct Chord
    {
        Chord
        (
            Degree degree = Degree::invalid,
            ChordQuality qualities = ChordQuality::none,
            Accidental accidental = Accidental::natural
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
