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

    struct TempoChange
    {
        Tempo targetTempo;
        Duration transitionDuration;
    };

    struct HarmonicTransition
    {
        Duration maxTransitionDuration;
        Interlude* transition;
        Scale* targetScale;
        Graph* targetGraph;
        Edge* targetEntryEdge;
    };

    enum EventType
    {
        NoEvent = 0,
        AddMotif = 1 << 0,
        RemoveMotif = 1 << 1,
        RequestInterlude = 1 << 2,
        CancelInterlude = 1 << 3,
        ChangeTempo = 1 << 4,
        ChangeScale = 1 << 5,
        ChangeGraph = 1 << 6,
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
