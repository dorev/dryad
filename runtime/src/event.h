#pragma once

#include "types.h"
#include "time.h"

namespace Dryad
{
    class Interlude;
    class Scale;
    class Graph;
    class Motif;
    class Edge;

    class TempoChange
    {
        // Only linear change
        Tempo targetTempo;
        Duration transitionDuration;
    };

    class HarmonicTransition
    {
        Duration maxTransitionDuration;
        Interlude* transition;
        Scale* targetScale;
        Graph* targetGraph;
        Edge* targetEntryEdge;
    };

    enum EventType : UInt8
    {
        AddMotif,
        RemoveMotif,
        RequestInterlude,
        CancelInterlude,
        ChangeTempo,
        ChangeScale,
        ChangeGraph,
    };

    using EventData = Variant
    <
        Motif*,
        Interlude*,
        TempoChange,
        HarmonicTransition
    >;

    struct Event
    {
        EventType type;
        EventData data;
    };
}
