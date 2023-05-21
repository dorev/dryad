#pragma once

#include "types.h"
#include "time.h"
#include "flags.h"

namespace Dryad
{
    class Scale;
    class Graph;
    class Motif;
    class Edge;

    struct TempoChange
    {
        Tempo targetTempo;
        Duration transitionDuration;
    };

    struct HarmonicTransition
    {
        Duration maxTransitionDuration;
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

    enum class EventType
    {
        NoEvent = 0,
        FLAG(AddMotif, 0),
        FLAG(RemoveMotif, 1),
        FLAG(RequestInterlude, 2),
        FLAG(CancelInterlude, 3),
        FLAG(ChangeTempo, 4),
        FLAG(ChangeScale, 5),
        FLAG(ChangeGraph, 6),
    };

    using EventData = Variant
    <
        Motif*,
        TempoChange,
        HarmonicTransition
    >;

    struct Event
    {
        EventType type;
        EventData data;
    };
}
