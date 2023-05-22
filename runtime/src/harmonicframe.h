#pragma once

#include "types.h"
#include "result.h"
#include "time.h"

namespace Dryad
{
    class Scale;
    class Motif;
    class Node;
    class Graph;

    // The HarmonicFrame represents all the harmony modifiers at a certain
    // point in the score generation process. It holds everything that is
    // required to generate the next few notes
    class HarmonicFrame
    {
    public:
        HarmonicFrame
        (
            TimeSignature timeSignature = TimeSignature(4, 4),
            Tempo tempo = DefaultTempo,
            ScoreTime frameStart = ScoreTime(1, 4, 0.0f),
            ScoreTime duration = ScoreTime(4, 4),
            Map<Motif*, UInt32> motifLevels = {},
            Scale* scale = nullptr,
            Node* node = nullptr,
            Graph* graph = nullptr
        );

        // Used when initializing the first harmonic frame
        Result MatchNode(Node* node);

        TimeSignature timeSignature;
        Tempo tempo;
        ScoreTime frameStart;
        ScoreTime duration;
        Map<Motif*, UInt32> motifLevels;
        Scale* scale;
        Node* node;
        Graph* graph;
    };
}
