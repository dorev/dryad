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

    class HarmonicContext
    {
    public:


    private:
        TimeSignature timeSignature;
        Position position;
        Tempo tempo;
        Scale* scale;
        Map<Motif*, UInt32> motifLevels;
        Node* node;
        Graph* graph;
    };
}
