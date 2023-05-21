#pragma once

#include "types.h"
#include "result.h"
#include "event.h"
#include "flags.h"

namespace Dryad
{
    class Motif;
    class Interlude;

    struct EventSummary
    {
        Map<Motif*, Int32> motifVariations;
        TempoChange tempoChangeRequested;
        Interlude* interludeRequested;
        HarmonicTransition harmonicTransitionRequested;
        EventType eventFlags;

        inline bool HasMotifChanges() const
        {
            return AnyFlagIsSet(EventType::AddMotif, EventType::RemoveMotif);
        }

        inline bool HasTempoChanges() const
        {
            return AnyFlagIsSet(EventType::ChangeTempo);
        }

        inline bool HasHarmonicChanges() const
        {
            return AnyFlagIsSet
            (
                EventType::RequestInterlude,
                EventType::CancelInterlude,
                EventType::ChangeGraph,
                EventType::ChangeScale
            );
        }
    };
}
