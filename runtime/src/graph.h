#pragma once

#include "types.h"
#include "time.h"
#include "chord.h"
#include "node.h"
#include "edge.h"

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

    using GraphPtr = SharedPtr<Graph>;
}
