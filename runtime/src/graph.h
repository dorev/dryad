#pragma once

#include "types.h"
#include "time.h"
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
        Duration CalculateMaxDuration()
        {

            return MusicTime();
        }
    private:
        Vector<Node*> nodes;
        Vector<Edge*> entryEdges;
        Vector<Edge*> exitEdges;
        TimeSignature timeSignature;
        NoteRange noteRange;
    };
}
