#pragma once

#include "types.h"
#include "result.h"
#include "event.h"
#include "eventsummary.h"
#include "flags.h"

namespace Dryad
{

    class EventReducer
    {
    public:
        Result Consume(EventType eventType, Motif* motif);
        Result Consume(EventType eventType, TempoChange tempoChange);
        Result Consume(EventType eventType, HarmonyTransition harmonicTransition);

        bool HasChanges();
        EventSummary DumpAndReset();

    private:
        EventSummary m_Summary;
    };
}
