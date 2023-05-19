#pragma once

#include "types.h"
#include "result.h"
#include "event.h"

namespace Dryad
{

    struct EventSummary
    {
        Map<Motif*, Int32> motifVariations;
        TempoChange tempoChangeRequested;
        Interlude* interludeRequested;
        HarmonicTransition harmonicTransitionRequested;
        EventType eventFlags;
    };

    class EventAccumulator
    {
    public:
        Result Consume(EventType eventType, Motif* motif);
        Result Consume(EventType eventType, Interlude* interlude);
        Result Consume(EventType eventType, TempoChange tempoChange);
        Result Consume(EventType eventType, HarmonicTransition harmonicTransition);

        bool HasChanges();
        EventSummary DumpAndReset();

    private:
        EventSummary _summary;
    };
}
