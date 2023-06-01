#pragma once

#include "types.h"
#include "result.h"
#include "scoretime.h"

namespace Dryad
{
    class Scale;
    class Motif;
    class Node;
    class Graph;

    // The HarmonicFrame represents all the harmony modifiers at a certain
    // point in the score generation process. It holds everything that is
    // required to generate the next few notes
    class HarmonyFrame
    {
    public:
        HarmonyFrame
        (
            TimeSignature timeSignature = TimeSignature(4, 4),
            Tempo tempo = DefaultTempo,
            ScoreTime frameStart = ScoreTime(1, 4, 0.0f),
            ScoreTime duration = ScoreTime(4, 4),
            Map<Motif*, UInt32> motifLevels = {},
            const Scale* scale = nullptr,
            Node* node = nullptr,
            Graph* graph = nullptr
        );

        // Used when initializing the first harmonic frame
        Result UpdateFromNode(Node* node);

        TimeSignature timeSignature;
        Tempo tempo;
        ScoreTime frameStart;
        ScoreTime duration;
        Map<Motif*, UInt32> motifLevels;
        const Scale* scale;
        Node* node;
        Graph* graph;

        // Eventually implement a memento mechanism if we want to backtrack?
        // What do we do when we asked for a strong transition, followed by another transition
        // that brings us to a frame similar to the initial one... how do we "cancel"?
        // We might have to backtrack on the score or keep more "progress" data in the frames

        // This?:
        // Probably the frames should stored in a tree, branching on the graph forced transitions.
        // This could be relevant especially when a transition is requested but not acted yet.
    };
}
