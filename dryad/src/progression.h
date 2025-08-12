#pragma once

#include "types.h"
#include "graph.h"
#include "chord.h"
#include "constants.h"

namespace Dryad
{

    class Motif;
    class Voice;
    class Scale;

    class ProgressionNode : public Node
    {
    public:

        ProgressionNode()
            : next(nullptr)
        {
        }

        ProgressionNode* next;
    };

    class ProgressionChord;
    class Progression : public Node
    {
    public:
        DRYAD_NODE_CLASS_ID(Progression);

        ProgressionNode* entryNode;
        ProgressionChord* currentProgressionChord;
        Vector<ProgressionNode*> nodes;
    };

    class ProgressionSwitchSequence : public ProgressionNode
    {
    public:
        DRYAD_NODE_CLASS_ID(ProgressionSwitchSequence);

        Vector<ProgressionNode*> outputs;
        int outputIndex;
    };

    struct MotifChange
    {
        Motif* oldMotif;
        Motif* newMotif;
    };

    struct VoiceChange
    {
        Voice* oldVoice;
        Voice* newVoice;
    };

    class ProgressionEvent : public ProgressionNode
    {
    public:
        DRYAD_NODE_CLASS_ID(ProgressionEvent);

        Progression* progressionChange;
        Scale* scaleChange;
        Vector<MotifChange> motifChanges;
        Vector<VoiceChange> voiceChanges;
        bool scoreEnd;
    };

    class ProgressionChord : public ProgressionNode
    {
    public:
        DRYAD_NODE_CLASS_ID(ProgressionChord);

        ProgressionChord(Chord chord, Time duration)
            : chord(chord)
            , duration(duration)
        {
        }

        Chord chord;
        Time duration;
    };

    class ProgressionChordInstance : public Node
    {
    public:
        DRYAD_NODE_CLASS_ID(ProgressionChordInstance);

        ProgressionChord* progressionChord;
        NoteValue root;
        Time relativePosition;
    };

} // namespace Dryad
