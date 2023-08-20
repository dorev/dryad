#pragma once

#include "types.h"
#include "flags.h"
#include "harmonytransition.h"

namespace Dryad
{
    class Scale;
    class Graph;
    class Motif;
    class Edge;

    struct TempoChange
    {
        Tempo targetTempo;
        // For a first implementation, only support instantaneous tempo changes
        // ScoreTime transitionDuration;
    };

    enum class EventType
    {
        NoEvent = 0,
        AddMotif,
        RemoveMotif,
        ChangeTempo,
        ChangeScale,
        ChangeGraph
    };

    using EventData = Variant
    <
        Motif*,
        TempoChange,
        HarmonyTransition
    >;

    struct Event
    {
        EventType type;
        EventData data;
    };
}
