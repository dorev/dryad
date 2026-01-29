#pragma once

#include "graph.h"
#include "constants.h"

namespace Dryad
{

    enum class HarmonicAnchor
    {
        Scale,
        Chord
    };

    enum class AnchorRhythmic
    {
        ChordBeginning,
        StrongBeat,
        WeakBeat,
        AnyBeat
    };

    enum class NoteIntervalType
    {
        Diatonic,
        Chromatic
    };

    class Motif;
    class MotifInstance;
    class ScoreFrame;
    class Voice;

    class MotifNote : public Node
    {
    public:
        DRYAD_NODE_CLASS_ID(MotifNote);

        MotifNote(Motif* motif, NoteRelative relativeValue, Time duration, Time offset);

        NoteRelative relativeValue;
        Time duration;
        Time offset;
    };


    class NoteInstance : public Node
    {
    public:
        DRYAD_NODE_CLASS_ID(NoteInstance);

        NoteInstance(NoteValue value, Time duration);

        NoteValue value;
        Time duration;
    };


    class Motif : public Node
    {
    public:
        DRYAD_NODE_CLASS_ID(Motif);

        HarmonicAnchor harmonicAnchor;
        AnchorRhythmic rhythmicAnchor;
        NoteIntervalType noteIntervalType;
        Time duration;

        MotifNote* addNote(NoteRelative relativeValue, Time duration, Time offset);

        // Evaluates the duration of the motif by checking the end time
        // of each note. The duration can be specified beyond that time
        // so the silence at the end of the motif is also included
        void updateDuration();

        // A motifNote should never be reused once it's destroyed
        bool destroyNote(MotifNote* motifNoteToRemove);

        MotifInstance* getLastInstance();
        Error getInstancesEndTime(Voice* voice, Time& time);

    };

    class MotifInstance : public Node
    {
    public:
        DRYAD_NODE_CLASS_ID(MotifInstance);

        MotifInstance(Time position);

        Time getEndTime();

        Time position;
    };

} // namespace Dryad
