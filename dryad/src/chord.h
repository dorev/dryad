#pragma once

#include "types.h"
#include "flag.h"

DRYAD_DECLARE_FLAG_ENUM(dryad_degree, unsigned)
{
    invalid = 0,
    DRYAD_FLAG(tonic, 1),
    root = tonic,
    DRYAD_FLAG(supertonic, 2),
    second = supertonic,
    DRYAD_FLAG(mediant, 3),
    third = mediant,
    DRYAD_FLAG(subdominant, 4),
    fourth = subdominant,
    DRYAD_FLAG(dominant, 5),
    fifth = dominant,
    DRYAD_FLAG(submediant, 6),
    sixth = submediant,
    DRYAD_FLAG(leading_tone, 7),
    seventh = leading_tone
};

DRYAD_DECLARE_FLAG_ENUM(dryad_chord_quality, unsigned)
{
    default = 0,
    minor = 1,
    major = 2,
    half_diminished = 3,
    augmented = 4,
    sus2 = 5,
    sus4 = 6,
    diminished = 7,
    DRYAD_FLAG(seventh_minor, 4),
    DRYAD_FLAG(seventh_major, 5),
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
};

enum class dryad_accidental
{
    natural,
    flat,
    sharp,
};

struct dryad_chord
{
    dryad_chord
    (
        dryad_degree degree,
        dryad_chord_quality qualities = dryad_chord_quality::default,
        dryad_accidental accidental = dryad_accidental::natural
    )
        : degree(degree)
        , qualities(qualities)
        , accidental(accidental)
    {
    }

    dryad_degree degree;
    dryad_chord_quality qualities;
    dryad_accidental accidental;
};
