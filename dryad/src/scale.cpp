#include "scale.h"

dryad_scale_note_offsets::dryad_scale_note_offsets
(
    dryad_note_relative tonic,
    dryad_note_relative supertonic,
    dryad_note_relative mediant,
    dryad_note_relative subdominant,
    dryad_note_relative dominant,
    dryad_note_relative submediant,
    dryad_note_relative leading_tone
)
    : degrees
    {
        tonic,
        supertonic,
        mediant,
        subdominant,
        dominant,
        submediant,
        leading_tone
    }
{
}

dryad_scale_degree_qualities::dryad_scale_degree_qualities
(
    dryad_chord_quality tonic,
    dryad_chord_quality supertonic,
    dryad_chord_quality mediant,
    dryad_chord_quality subdominant,
    dryad_chord_quality dominant,
    dryad_chord_quality submediant,
    dryad_chord_quality leading_tone
)
    : degrees
    {
        tonic,
        supertonic,
        mediant,
        subdominant,
        dominant,
        submediant,
        leading_tone
    }
{
}

dryad_scale::dryad_scale
(
    dryad_scale_note_offsets scale_note_offsets,
    dryad_scale_degree_qualities degree_qualities
)
    : note_offsets(scale_note_offsets)
    , degree_qualities(degree_qualities)
{
}

dryad_scale::dryad_scale(const dryad_scale& other)
    : note_offsets(other.note_offsets)
    , degree_qualities(other.degree_qualities)
{
}

dryad_scale& dryad_scale::operator=(const dryad_scale& other)
{
    if (&other != this)
    {
        note_offsets = other.note_offsets;
        degree_qualities = other.degree_qualities;
    }

    return *this;
}

dryad_note_relative dryad_scale::get_degree_note_offset(dryad_degree degree)
{
    return note_offsets.degrees[static_cast<int>(degree) - 1];
}

dryad_chord_quality dryad_scale::get_degree_chord_quality(dryad_degree degree)
{
    return degree_qualities.degrees[static_cast<int>(degree) - 1];
}
