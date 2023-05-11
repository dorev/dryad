#pragma once

#include "types.h"
#include "chord.h"

namespace Dryad
{
    class Graph
    {
    public:
        Result InsertNode(NodePtr node);
        Result RemoveNode(NodePtr node);
        EdgePtr AddEdge(NodePtr sourceNode, NodePtr destinationNode);
        
    private:
        Vector<NodePtr> nodes;
        Vector<EdgePtr> entryEdges;
        Vector<EdgePtr> exitEdges;
        TimeSignature timeSignature;
        NoteRange noteRange;
    };
}
