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
