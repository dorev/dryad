#pragma once

#include "types.h"
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
        Chord chord;
        ScoreTime duration;
        NoteRange noteRange;
        void* userData;

        bool IsValid() const
        {
            return graph != nullptr
                && ((edgesIn.Empty() && graphEntry)
                    || (edgesOut.Empty() && graphExit)
                    || (!edgesIn.Empty() && !edgesOut.Empty()))
                && duration > 0;
        }
    };
}
