#pragma once

#include "types.h"
#include "scoretime.h"
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
        ScoreTime transitionDuration;
    };

    enum class EventType
    {
        NoEvent = 0,
        DRYAD_FLAG(AddMotif, 0),
        DRYAD_FLAG(RemoveMotif, 1),
        DRYAD_FLAG(RequestInterlude, 2),
        DRYAD_FLAG(CancelInterlude, 3),
        DRYAD_FLAG(ChangeTempo, 4),
        DRYAD_FLAG(ChangeScale, 5),
        DRYAD_FLAG(ChangeGraph, 6),
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
