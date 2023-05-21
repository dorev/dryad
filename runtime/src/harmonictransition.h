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
        ScoreTime maxTransitionDuration;
        Scale* targetScale;
        Graph* targetGraph;
        Edge* targetEntryEdge;

        // Take other's values without replacing valid pointer with nullptr
        void Merge(const HarmonicTransition& other)
        {
            maxTransitionDuration = other.maxTransitionDuration;
            if(other.targetScale != nullptr)
            {
                targetScale = other.targetScale;
            }
            if(other.targetGraph != nullptr)
            {
                targetGraph = other.targetGraph;
            }
            if(other.targetEntryEdge != nullptr)
            {
                targetEntryEdge = other.targetEntryEdge;
            }
        }
    };
}
