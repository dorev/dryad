#pragma once

#include "types.h"
#include "time.h"
#include "flags.h"

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
