#include "scale.h"

namespace Dryad
{

    ScaleNoteOffsets::ScaleNoteOffsets
    (
        NoteRelative tonic,
        NoteRelative supertonic,
        NoteRelative mediant,
        NoteRelative subdominant,
        NoteRelative dominant,
        NoteRelative submediant,
        NoteRelative leadingTone
    )
        : degrees
        {
            tonic,
            supertonic,
            mediant,
            subdominant,
            dominant,
            submediant,
            leadingTone
        }
    {
    }

    ScaleDegreeQualities::ScaleDegreeQualities
    (
        ChordQuality tonic,
        ChordQuality supertonic,
        ChordQuality mediant,
        ChordQuality subdominant,
        ChordQuality dominant,
        ChordQuality submediant,
        ChordQuality leadingTone
    )
        : degrees
        {
            tonic,
            supertonic,
            mediant,
            subdominant,
            dominant,
            submediant,
            leadingTone
        }
    {
    }

    Scale::Scale
    (
        ScaleNoteOffsets noteOffsets,
        ScaleDegreeQualities degreeQualities
    )
        : noteOffsets(noteOffsets)
        , degreeQualities(degreeQualities)
    {
    }

    Scale::Scale(const Scale& other)
        : noteOffsets(other.noteOffsets)
        , degreeQualities(other.degreeQualities)
    {
    }

    Scale& Scale::operator=(const Scale& other)
    {
        if (&other != this)
        {
            noteOffsets = other.noteOffsets;
            degreeQualities = other.degreeQualities;
        }

        return *this;
    }

    NoteRelative Scale::getDegreeNoteOffset(Degree degree)
    {
        return noteOffsets.degrees[static_cast<int>(degree) - 1];
    }

    ChordQuality Scale::getDegreeChordQuality(Degree degree)
    {
        return degreeQualities.degrees[static_cast<int>(degree) - 1];
    }

} // namespace Dryad
