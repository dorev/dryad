#pragma once

#include "types.h"
#include "result.h"
#include "node.h"

namespace Dryad
{
    class Scale;
    class Graph;
    class Edge;

    struct HarmonyTransition
    {
        ScoreTime maxDuration;
        const Scale* scale;
        const Graph* graph;
        const Edge* entryEdge;

        HarmonyTransition()
            : maxDuration(0)
            , scale(nullptr)
            , graph(nullptr)
            , entryEdge(nullptr)
        {
        }

        // Take other object values without replacing a valid pointer with nullptr
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

        Node* GetEntryNode()
        {
            if(entryEdge != nullptr)
            {
                return entryEdge->destination;
            }
            return nullptr;
        }
    };
}
