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
            // This will be ignored for the moment
            return false;
            //return AnyFlagIsSet(EventType::ChangeTempo);
        }

        bool HasHarmonyChanges() const
        {
            return AnyFlagIsSet
            (
                eventFlags,
                EventType::RequestInterlude,
                EventType::CancelInterlude,
                EventType::ChangeGraph,
                EventType::ChangeScale
            );
        }
    };
}
