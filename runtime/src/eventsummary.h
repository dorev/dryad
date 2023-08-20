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
        Map<const Motif*, Int32> motifVariations;
        TempoChange tempoChangeRequested;
        HarmonyTransition harmonyTransitionRequested;
        EventType eventFlags;

        EventSummary()
            : motifVariations()
            , tempoChangeRequested()
            , harmonyTransitionRequested()
            , eventFlags(EventType::NoEvent)
        {
        }

        bool HasMotifChanges() const
        {
            return AnyFlagIsSet
            (
                eventFlags,
                EventType::AddMotif,
                EventType::RemoveMotif
            );
        }

        bool HasTempoChanges() const
        {
            return AnyFlagIsSet(EventType::ChangeTempo);
        }

        bool HasHarmonyChanges() const
        {
            return AnyFlagIsSet
            (
                eventFlags,
                EventType::ChangeGraph,
                EventType::ChangeScale
            );
        }
    };
}
