#pragma once

#include "types.h"
#include "chord.h"
#include "motif.h"

namespace Dryad
{

    struct SerializedNote
    {
        Time position;
        Time duration;
        NoteValue noteValue;
    };

    struct SerializedVoice
    {
        String name;
        int id;
        Vector<SerializedNote> notes;
    };

    struct SerializedScore
    {
        Vector<SerializedVoice> voices;
    };

    struct SerializedMotifNote
    {
        NoteRelative relativeValue;
        Time duration;
        Time offset;
    };

    struct SerializedMotif
    {
        HarmonicAnchor harmonicAnchor;
        AnchorRhythmic rhythmicAnchor;
        NoteIntervalType noteIntervalType;
        Time duration;
        Vector<SerializedMotifNote> notes;
    };

    struct SerializedVoiceDefinition
    {
        int id;
        String name;
        Vector<int> motifIndices;
    };

    struct SerializedMotifChange
    {
        int oldMotifIndex;
        int newMotifIndex;
    };

    struct SerializedVoiceChange
    {
        int oldVoiceIndex;
        int newVoiceIndex;
    };

    enum class SerializedProgressionNodeType
    {
        Chord,
        Event,
        SwitchSequence
    };

    struct SerializedProgressionNode
    {
        SerializedProgressionNodeType type;
        int nextIndex;
        Vector<int> outputs;
        Chord chord;
        Time duration;
        bool scoreEnd;
        int progressionChangeIndex;
        bool hasScaleChange;
        NoteRelative scaleOffsets[7]{};
        ChordQuality scaleDegreeQualities[7]{};
        Vector<SerializedMotifChange> motifChanges;
        Vector<SerializedVoiceChange> voiceChanges;
    };

    struct SerializedProgression
    {
        Vector<SerializedProgressionNode> nodes;
        int entryIndex;
    };

    class IExporter
    {
    public:
        virtual ~IExporter()
        {
        }

        virtual Error exportScore(const SerializedScore& score, String export_path) = 0;
    };

} // namespace Dryad
