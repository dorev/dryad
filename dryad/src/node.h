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
                    || (edgesIn.NotEmpty() && edgesOut.NotEmpty()))
                && duration > 0;
        }

        bool IsNotValid() const
        {
            return !IsValid();
        }

        Node* GetNext() const
        {
            if (edgesOut.NotEmpty())
            {
                if (edgesOut.Size() == 1)
                {
                    return edgesOut[0]->destination;
                }
                else
                {
                    Edge* edge = Edge::SelectWeightedRandom(edgesOut);
                    if (edge != nullptr)
                    {
                        return edge->destination;
                    }
                }
            }
            return nullptr;
        }
    };
}
