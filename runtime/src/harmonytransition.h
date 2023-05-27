#pragma once

#include "types.h"
#include "result.h"
#include "time.h"

#include "node.h"

namespace Dryad
{
    class Scale;
    class Graph;
    class Edge;

    struct HarmonyTransition
    {
        ScoreTime maxDuration;
        Scale* scale;
        Graph* graph;
        Edge* entryEdge;

        // Take other's values without replacing valid pointer with nullptr
        void Merge(const HarmonyTransition& other)
        {
            maxDuration = other.maxDuration;
            if(other.scale != nullptr)
            {
                scale = other.scale;
            }
            if(other.graph != nullptr)
            {
                graph = other.graph;
            }
            if(other.entryEdge != nullptr)
            {
                entryEdge = other.entryEdge;
            }
        }

        Node* GetTargetNode()
        {
            if(entryEdge != nullptr)
            {
                return entryEdge->destination;
            }
            return nullptr;
        }
    };
}
