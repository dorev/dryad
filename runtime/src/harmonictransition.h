#pragma once

#include "types.h"
#include "result.h"
#include "time.h"

namespace Dryad
{
    class Scale;
    class Graph;
    class Edge;

    struct HarmonicTransition
    {
        ScoreTime maxDuration;
        Scale* scale;
        Graph* graph;
        Edge* entryEdge;

        // Take other's values without replacing valid pointer with nullptr
        void Merge(const HarmonicTransition& other)
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
    };
}
