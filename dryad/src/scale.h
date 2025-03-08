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

    inline static const dryad_scale_degree_qualities mixolydian_degree_qualitie
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
    inline static const dryad_scale mixolydian_scale{mixolydian_offsets, mixolydian_degree_qualitie};
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
    dryad_note_relative degree_offset_from_root = scale->note_offsets[degree];

    // CATCH UP HERE!!!

    // TODO: Prepare offsets of default chord

    // NOTE:
    // default scale qualities will bring in seventh notes, is that something we want or should sevenths be explicitely added (even for fifth degree)?
    if (qualities != dryad_chord_quality::default)
    {
        if (bool(qualities & dryad_chord_quality::major))
        {

        }
        if (bool(qualities & dryad_chord_quality::minor))
        {

        }
        if (bool(qualities & dryad_chord_quality::half_diminished))
        {

        }
        if (bool(qualities & dryad_chord_quality::augmented))
        {

        }
        if (bool(qualities & dryad_chord_quality::sus2))
        {

        }
        if (bool(qualities & dryad_chord_quality::sus4))
        {

        }
        if (bool(qualities & dryad_chord_quality::diminished))
        {

        }
        if (bool(qualities & dryad_chord_quality::seventh_minor))
        {

        }
        if (bool(qualities & dryad_chord_quality::seventh_major))
        {

        }
        if (bool(qualities & dryad_chord_quality::add6))
        {

        }
        if (bool(qualities & dryad_chord_quality::add9))
        {

        }
        if (bool(qualities & dryad_chord_quality::add11))
        {

        }
        if (bool(qualities & dryad_chord_quality::add13))
        {

        }
        if (bool(qualities & dryad_chord_quality::flat2))
        {

        }
        if (bool(qualities & dryad_chord_quality::flat3))
        {

        }
        if (bool(qualities & dryad_chord_quality::flat4))
        {

        }
        if (bool(qualities & dryad_chord_quality::flat5))
        {

        }
        if (bool(qualities & dryad_chord_quality::flat6))
        {
        
        }
        if (bool(qualities & dryad_chord_quality::flat7))
        {

        }
        if (bool(qualities & dryad_chord_quality::flat9))
        {

        }
        if (bool(qualities & dryad_chord_quality::flat11))
        {
        
        }
        if (bool(qualities & dryad_chord_quality::flat13))
        {

        }
        if (bool(qualities & dryad_chord_quality::sharp2))
        {

        }
        if (bool(qualities & dryad_chord_quality::sharp3))
        {

        }
        if (bool(qualities & dryad_chord_quality::sharp4))
        {

        }
        if (bool(qualities & dryad_chord_quality::sharp5))
        {

        }
        if (bool(qualities & dryad_chord_quality::sharp6))
        {
        
        }
        if (bool(qualities & dryad_chord_quality::sharp7))
        {

        }
        if (bool(qualities & dryad_chord_quality::sharp9))
        {

        }
        if (bool(qualities & dryad_chord_quality::sharp11))
        {
        
        }
        if (bool(qualities & dryad_chord_quality::sharp13))
        {

        }
    }

    // TODO: shift everything based on accidental (maybe accidental should become a dryad_note_relative!!)

    return dryad_error_not_implemented;
}
