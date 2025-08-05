#pragma once

#include "chord.h"
#include "graph.h"

struct dryad_scale_note_offsets
{
    dryad_scale_note_offsets
    (
        dryad_note_relative tonic,
        dryad_note_relative supertonic,
        dryad_note_relative mediant,
        dryad_note_relative subdominant,
        dryad_note_relative dominant,
        dryad_note_relative submediant,
        dryad_note_relative leading_tone
    );

    inline dryad_note_relative operator[](dryad_degree degree) const
    {
        if (degree >= dryad_degree::tonic && degree < dryad_degree::limit)
            return degrees[static_cast<int>(degree)];

        return static_cast<dryad_note_relative>(dryad_invalid);
    }

    dryad_note_relative degrees[7];
};

struct dryad_scale_degree_qualities
{
    dryad_scale_degree_qualities
    (
        dryad_chord_quality tonic,
        dryad_chord_quality supertonic,
        dryad_chord_quality mediant,
        dryad_chord_quality subdominant,
        dryad_chord_quality dominant,
        dryad_chord_quality submediant,
        dryad_chord_quality leading_tone
    );

    inline dryad_chord_quality operator[](dryad_degree degree) const
    {
        if (degree >= dryad_degree::tonic && degree < dryad_degree::limit)
            return degrees[static_cast<int>(degree)];

        return dryad_chord_quality::invalid;
    }

    dryad_chord_quality degrees[7];
};

class dryad_scale : public dryad_node
{
public:
    DRYAD_NODE_CLASS_ID(dryad_scale);

    dryad_scale(dryad_scale_note_offsets scale_note_offsets, dryad_scale_degree_qualities degree_qualities);

    dryad_scale(const dryad_scale& other);

    dryad_scale& operator=(const dryad_scale& other);

    dryad_note_relative get_degree_note_offset(dryad_degree degree);
    dryad_chord_quality get_degree_chord_quality(dryad_degree degree);

    dryad_scale_note_offsets note_offsets;
    dryad_scale_degree_qualities degree_qualities;
};

struct dryad_scale_library
{
    inline static const dryad_scale_note_offsets ionian_offsets{0, 2, 4, 5, 7, 9, 11};
    inline static const dryad_scale_note_offsets major_offsets = ionian_offsets;
    inline static const dryad_scale_note_offsets dorian_offsets{0, 2, 3, 5, 7, 9, 10};
    inline static const dryad_scale_note_offsets phrygian_offsets{0, 1, 3, 5, 7, 8, 10};
    inline static const dryad_scale_note_offsets lydian_offsets{0, 2, 4, 6, 7, 9, 11};
    inline static const dryad_scale_note_offsets mixolydian_offsets{0, 2, 4, 5, 7, 9, 10};
    inline static const dryad_scale_note_offsets aeolian_offsets{0, 2, 3, 5, 7, 8, 10};
    inline static const dryad_scale_note_offsets minor_natural_offsets = aeolian_offsets;
    inline static const dryad_scale_note_offsets locrian_offsets{0, 1, 3, 5, 6, 8, 10};
    inline static const dryad_scale_note_offsets minor_harmonic_offsets{0, 2, 3, 5, 7, 8, 11};
    inline static const dryad_scale_note_offsets minor_melodic_offsets{0, 2, 3, 5, 7, 9, 11};

    inline static const dryad_scale_degree_qualities ionian_degree_qualities
    {
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::half_diminished
    };
    
    inline static const dryad_scale_degree_qualities major_degree_qualities = ionian_degree_qualities;

    inline static const dryad_scale_degree_qualities dorian_degree_qualities
    {
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::half_diminished,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major
    };

    inline static const dryad_scale_degree_qualities phrygian_degree_qualities
    {
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::half_diminished,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor
    };

    inline static const dryad_scale_degree_qualities lydian_degree_qualities
    {
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::half_diminished,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor
    };

    inline static const dryad_scale_degree_qualities mixolydian_degree_qualities
    {
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::half_diminished,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor
    };

    inline static const dryad_scale_degree_qualities aeolian_degree_qualities
    {
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::diminished,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor
    };

    inline static const dryad_scale_degree_qualities minor_natural_degree_qualities = aeolian_degree_qualities;

    inline static const dryad_scale_degree_qualities locrian_degree_qualities
    {
        dryad_chord_quality::half_diminished,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor
    };

    inline static const dryad_scale_degree_qualities minor_harmonic_degree_qualities
    {
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::diminished,
        dryad_chord_quality::augmented,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::diminished
    };

    inline static const dryad_scale_degree_qualities minor_melodic_degree_qualities
    {
        dryad_chord_quality::minor | dryad_chord_quality::seventh_major,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::augmented,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::diminished,
        dryad_chord_quality::diminished
    };

    inline static const dryad_scale ionian_scale{ionian_offsets, ionian_degree_qualities};
    inline static const dryad_scale major_scale{ionian_scale};
    inline static const dryad_scale dorian_scale{dorian_offsets, dorian_degree_qualities};
    inline static const dryad_scale phrygian_scale{phrygian_offsets, phrygian_degree_qualities};
    inline static const dryad_scale lydian_scale{lydian_offsets, lydian_degree_qualities};
    inline static const dryad_scale mixolydian_scale{mixolydian_offsets, mixolydian_degree_qualities};
    inline static const dryad_scale aeolian_scale{aeolian_offsets, aeolian_degree_qualities};
    inline static const dryad_scale minor_natural_scale{aeolian_scale};
    inline static const dryad_scale locrian_scale{locrian_offsets, locrian_degree_qualities};
    inline static const dryad_scale minor_harmonic_scale{minor_harmonic_offsets, minor_harmonic_degree_qualities};
    inline static const dryad_scale minor_melodic_scale{minor_melodic_offsets, minor_melodic_degree_qualities};
};

inline dryad_error get_chord_offsets_from_root(const dryad_chord& chord, const dryad_scale* scale, dryad_vector<dryad_note_relative>& offsets)
{
    if (!scale)
        return dryad_error_invalid_scale;

    if (chord.degree == dryad_degree::invalid)
        return dryad_error_invalid_degree;

    dryad_degree degree = chord.degree;

    dryad_chord_quality qualities = scale->degree_qualities[degree];
    unsigned qualities_bits = static_cast<unsigned>(qualities);
    unsigned chord_bits = static_cast<unsigned>(chord.qualities);
    unsigned base_mask = 0x7;

    if (chord_bits & base_mask)
        qualities_bits = (qualities_bits & ~base_mask) | (chord_bits & base_mask);
    qualities_bits |= (chord_bits & ~base_mask);
    qualities = static_cast<dryad_chord_quality>(qualities_bits);

    dryad_note_relative degree_offset_from_root = scale->note_offsets[degree];

    offsets.clear();

    dryad_vector<dryad_note_relative> intervals;
    intervals.push_back(0);

    unsigned base_quality = qualities_bits & base_mask;

    switch (base_quality)
    {
    case static_cast<unsigned>(dryad_chord_quality::minor):
        intervals.push_back(3);
        intervals.push_back(7);
        break;
    case static_cast<unsigned>(dryad_chord_quality::major):
        intervals.push_back(4);
        intervals.push_back(7);
        break;
    case static_cast<unsigned>(dryad_chord_quality::half_diminished):
        intervals.push_back(3);
        intervals.push_back(6);
        intervals.push_back(10);
        break;
    case static_cast<unsigned>(dryad_chord_quality::augmented):
        intervals.push_back(4);
        intervals.push_back(8);
        break;
    case static_cast<unsigned>(dryad_chord_quality::sus2):
        intervals.push_back(2);
        intervals.push_back(7);
        break;
    case static_cast<unsigned>(dryad_chord_quality::sus4):
        intervals.push_back(5);
        intervals.push_back(7);
        break;
    case static_cast<unsigned>(dryad_chord_quality::diminished):
        intervals.push_back(3);
        intervals.push_back(6);
        break;
    default:
        break;
    }

    auto push_unique = [&](dryad_note_relative interval)
    {
        for (dryad_note_relative value : intervals)
            if (value == interval)
                return;
        intervals.push_back(interval);
    };

    if (bool(qualities & dryad_chord_quality::seventh_minor))
        push_unique(10);
    if (bool(qualities & dryad_chord_quality::seventh_major))
        push_unique(11);
    if (bool(qualities & dryad_chord_quality::add6))
        push_unique(9);
    if (bool(qualities & dryad_chord_quality::add9))
        push_unique(14);
    if (bool(qualities & dryad_chord_quality::add11))
        push_unique(17);
    if (bool(qualities & dryad_chord_quality::add13))
        push_unique(21);

    auto alter_interval = [&](dryad_note_relative interval, int delta) -> bool
    {
        for (dryad_note_relative& value : intervals)
        {
            if (value == interval)
            {
                value += delta;
                return true;
            }
        }
        return false;
    };

    auto alter_base = [&](dryad_note_relative interval, int delta) -> bool
    {
        if (alter_interval(interval, delta))
            return true;
        return alter_interval(interval + 12, delta);
    };

    if (bool(qualities & dryad_chord_quality::flat2))
        alter_base(2, -1);
    if (bool(qualities & dryad_chord_quality::flat3))
    {
        alter_base(4, -1);
        alter_base(3, -1);
    }
    if (bool(qualities & dryad_chord_quality::flat4))
        alter_base(5, -1);
    if (bool(qualities & dryad_chord_quality::flat5))
    {
        if (!alter_base(7, -1))
            alter_base(8, -1);
    }
    if (bool(qualities & dryad_chord_quality::flat6))
    {
        if (!alter_base(9, -1))
            alter_base(8, -1);
    }
    if (bool(qualities & dryad_chord_quality::flat7))
    {
        if (!alter_base(11, -1))
            alter_base(10, -1);
    }
    if (bool(qualities & dryad_chord_quality::flat9))
        alter_interval(14, -1);
    if (bool(qualities & dryad_chord_quality::flat11))
        alter_interval(17, -1);
    if (bool(qualities & dryad_chord_quality::flat13))
        alter_interval(21, -1);
    if (bool(qualities & dryad_chord_quality::sharp2))
        alter_base(2, 1);
    if (bool(qualities & dryad_chord_quality::sharp3))
    {
        alter_base(3, 1);
        alter_base(4, 1);
    }
    if (bool(qualities & dryad_chord_quality::sharp4))
        alter_base(5, 1);
    if (bool(qualities & dryad_chord_quality::sharp5))
    {
        if (!alter_base(7, 1))
            alter_base(6, 1);
    }
    if (bool(qualities & dryad_chord_quality::sharp6))
    {
        if (!alter_base(8, 1))
            alter_base(9, 1);
    }
    if (bool(qualities & dryad_chord_quality::sharp7))
    {
        if (!alter_base(10, 1))
            alter_base(11, 1);
    }
    if (bool(qualities & dryad_chord_quality::sharp9))
        alter_interval(14, 1);
    if (bool(qualities & dryad_chord_quality::sharp11))
        alter_interval(17, 1);
    if (bool(qualities & dryad_chord_quality::sharp13))
        alter_interval(21, 1);

    if (chord.accidental == dryad_accidental::flat)
        degree_offset_from_root--;
    else if (chord.accidental == dryad_accidental::sharp)
        degree_offset_from_root++;

    for (dryad_note_relative interval : intervals)
        offsets.push_back(degree_offset_from_root + interval);

    return dryad_success;
}
