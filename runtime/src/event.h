#pragma once

#include "types.h"
#include "time.h"

namespace Dryad
{
    class Interlude;
    class Scale;
    class Graph;
    class Motif;

    class TempoChange
    {
        // Only linear change
        Tempo targetTempo;
        Duration transitionDuration;
    };

    class GraphChange
    {
        Duration maxTransitionDuration;
        Interlude* transition;
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
        ToSilence,
    };

    using EventData = Variant
    <
        IdType,
        Graph*,
        Motif*,
        Interlude*,
        Scale*,
        Duration,
        TimeSignature,
        TempoChange,
        GraphChange
    >;

    struct Event
    {
        EventType type;
        EventData data;
    };
}
