#pragma once

#include "types.h"
#include "identification.h"

namespace Dryad
{

    class TempoChange
    {
        // Only linear change
        Tempo targetTempo;
        Duration transitionDuration;
    };

    class GraphChange
    {
        Duration maxTransitionDuration;
        InterludePtr transition;
    };

    enum EventType : UInt8
    {
        AddGraph,
        RemoveGraph,
        AddMotif,
        RemoveMotif,
        RequestInterlude,
        CancelInterlude,
        ChangeTempo,
        ChangeScale,
        ToSilence,
    };

    class Event : public Identifiable
    {
        using Data = Variant
        <
            IdType,
            GraphPtr,
            MotifPtr,
            InterludePtr,
            TimeSignature,
            TempoChange,
            Scale,
            Duration
        >;
        EventType type;
        Data data;
    };
}
