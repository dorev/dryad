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

    class HarmonicFrame
    {
    public:
        HarmonicFrame()
            : timeSignature(4,4)
            , duration()
            , tempo(120)
            , motifLevels()
            , scale(nullptr)
            , node(nullptr)
            , graph(nullptr)
        {
        }

        void SetDuration(Duration newDuration)
        {
            duration = newDuration;
        }

    private:
        TimeSignature timeSignature;
        Duration duration;
        Tempo tempo;
        Map<Motif*, UInt32> motifLevels;
        Scale* scale;
        Node* node;
        Graph* graph;
    };
}
