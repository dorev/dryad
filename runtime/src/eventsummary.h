#pragma once

#include "types.h"
#include "result.h"
#include "event.h"
#include "flags.h"

namespace Dryad
{
    class Motif;

    struct EventSummary
    {
        Map<const Motif*, Int32> motifsVariations;
        TempoChange tempoChangeRequested;
        HarmonyTransition harmonyTransitionRequested;
        EventType eventFlags;

        EventSummary()
            : motifsVariations()
            , tempoChangeRequested()
            , harmonyTransitionRequested()
            , eventFlags(EventType::NoEvent)
        {
        }

        bool HasMotifChanges() const
        {
            return Dryad::HasMotifChanges(eventFlags);
        }

        bool HasTempoChanges() const
        {
            return Dryad::HasTempoChanges(eventFlags);
        }

        bool HasHarmonyChanges() const
        {
            return Dryad::HasHarmonyChanges(eventFlags);
        }
    };
}
