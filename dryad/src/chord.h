#pragma once

#include "types.h"
#include "result.h"
#include "flag.h"
#include "constants.h"

namespace dryad
{
    enum class dryad_accidental
    {
        natural,
        flat,
        sharp,
    };

    enum class dryad_degree
    {
        DRYAD_FLAG(tonic, 0),
        DRYAD_FLAG(supertonic, 1),
        DRYAD_FLAG(mediant, 2),
        DRYAD_FLAG(subdominant, 3),
        DRYAD_FLAG(dominant, 4),
        DRYAD_FLAG(submediant, 5),
        DRYAD_FLAG(leading_tone, 6),
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
        DRYAD_FLAG(seventh, 4),
        DRYAD_FLAG(major_seventh, 5),
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

    constexpr dryad_chord_quality simlified_chord_qualities =
          dryad_chord_quality::minor
        | dryad_chord_quality::major
        | dryad_chord_quality::half_diminished
        | dryad_chord_quality::augmented
        | dryad_chord_quality::sus2
        | dryad_chord_quality::sus4
        | dryad_chord_quality::diminished;

    class dryad_scale;

    class dryad_chord
    {
    public:
        dryad_note_value root;
        dryad_degree degree;
        dryad_chord_quality qualities;
        dryad_accidental accidental;

        dryad_chord
        (
            dryad_note_value root = C,
            dryad_degree degree = dryad_degree::tonic,
            dryad_chord_quality qualities = dryad_chord_quality::default,
            dryad_accidental accidental = dryad_accidental::natural
        );

        bool operator==(const dryad_chord& other) const;
        bool is_dominant_of(dryad_chord other) const;
        static bool are_similar(const dryad_chord& left, const dryad_chord& right);
        Result apply_scale(const dryad_scale* scale);
    };
}
