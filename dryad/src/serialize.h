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
