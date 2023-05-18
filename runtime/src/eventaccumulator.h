#pragma once

#include "types.h"
#include "result.h"
#include "event.h"

namespace Dryad
{
    class EventAccumulator
    {
    public:
        Result Consume(EventType eventType, Motif* motif);
        Result Consume(EventType eventType, Interlude* interlude);
        Result Consume(EventType eventType, TempoChange tempoChange);
        Result Consume(EventType eventType, HarmonicTransition harmonicTransition);

    private:
        Map<Motif*, Int32> motifVariations;
        TempoChange tempoChangeRequested;
        Interlude* interludeRequested; // this is separated from harmonic transition to avoid breaking the structure of a graph change
        HarmonicTransition harmonicTransitionRequested;
    };
}
