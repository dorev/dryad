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
        Chord Chord;
        Duration duration;
        bool overrideGraphNoteRange;
        NoteRange noteRange;
        bool graphEntry;
        bool graphExit;
        void* userData;
    };
}
