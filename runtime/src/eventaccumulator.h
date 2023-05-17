#pragma once

#include "types.h"
#include "result.h"
#include "event.h"

namespace Dryad
{
    class EventAccumulator
    {
    public:
        Result Process(EventType eventType, Motif* motif);
        Result Process(EventType eventType, Interlude* interlude);
        Result Process(EventType eventType, TempoChange tempoChange);
        Result Process(EventType eventType, HarmonicTransition harmonicTransition);

    private:
        Map<Motif*, Int32> motifVariations;
        TempoChange tempoChange;
        Interlude* requestedIntelude; // this is separated from harmonic transition to avoid breaking the structure of a graph change
        HarmonicTransition harmonicTransition;
    };
}
