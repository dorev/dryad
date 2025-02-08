#include "scale.h"

dryad_scale_note_offsets::dryad_scale_note_offsets
(
    dryad_note_value tonic,
    dryad_note_value supertonic,
    dryad_note_value mediant,
    dryad_note_value subdominant,
    dryad_note_value dominant,
    dryad_note_value submediant,
    dryad_note_value leading_tone
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
