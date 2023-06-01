#pragma once

#include "types.h"
#include "chord.h"

namespace Dryad
{
    class Node;
    class Edge;

    class Graph
    {
    public:
        Result InsertNode(Node* node);
        Result RemoveNode(Node* node);
        Edge* AddEdge(Node* sourceNode, Node* destinationNode);

        // Do I need this?
        // See `modegraph.cpp` in 1st rewrite
        ScoreTime CalculateLongestCycle();
        ScoreTime CalculateShortestCycle();

        bool HasEntryEdge(const Edge* edge) const
        {
            return entryEdges.Contains(edge);
        }

        Vector<Node*> nodes;
        Vector<Edge*> entryEdges;
        Vector<Edge*> exitEdges;
        NoteRange noteRange;
    };
}
