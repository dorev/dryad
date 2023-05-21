#pragma once

#include "types.h"
#include "time.h"
#include "chord.h"
#include "edge.h"
#include "graph.h"

namespace Dryad
{
    class Node
    {
    public:
        Graph* graph;
        Vector<Edge*> edgesIn;
        Vector<Edge*> edgesOut;
        bool graphEntry;
        bool graphExit;
        Chord Chord;
        ScoreTime duration;
        NoteRange noteRange;
        void* userData;
    };
}
