#include "eventaccumulator.h"
#include "flags.h"

namespace Dryad
{
    Result EventAccumulator::Consume(EventType eventType, Motif* motif)
    {
        Int32* variationCount = nullptr;
        if(_summary.motifVariations.Find(motif, variationCount))
        {
            switch(eventType)
            {
                case AddMotif:
                    (*variationCount)++;
                    break;
                case RemoveMotif:
                    (*variationCount)--;
                    break;
                default:
                    return InvalidEventType;
            }
            if((*variationCount) == 0 && !_summary.motifVariations.Remove(motif))
            {
                return ConcurrencyError;
            }
        }
        else
        {
            switch(eventType)
            {
                case AddMotif:
                    _summary.motifVariations[motif] = 1;
                    break;
                case RemoveMotif:
                    break;
                default:
                    return InvalidEventType;
            }
        }
        SetFlag(_summary.eventFlags, eventType);
        return Success;
    }

    Result EventAccumulator::Consume(EventType eventType, Interlude* interlude)
    {
        switch(eventType)
        {
            case RequestInterlude:
                _summary.interludeRequested = interlude;
                SetFlag(_summary.eventFlags, RequestInterlude);
                break;
            case CancelInterlude:
                _summary.interludeRequested = nullptr;
                break;
            default:
                return InvalidEventType;
        }
        SetFlag(_summary.eventFlags, eventType);
        return Success;
    }

    Result EventAccumulator::Consume(EventType eventType, TempoChange tempoChange)
    {
        if(eventType == ChangeTempo)
        {
            _summary.tempoChangeRequested = tempoChange;
            SetFlag(_summary.eventFlags, eventType);
            return Success;
        }
        return InvalidEventType;
    }

    Result EventAccumulator::Consume(EventType eventType, HarmonicTransition harmonicTransition)
    {
        switch(eventType)
        {
            case ChangeScale:
                _summary.harmonicTransitionRequested.targetScale = harmonicTransition.targetScale;
                break;
            case ChangeGraph:
                _summary.interludeRequested = nullptr;
                break;
            default:
                return InvalidEventType;
        }
        SetFlag(_summary.eventFlags, eventType);
        return Success;
    }

    bool EventAccumulator::HasChanges()
    {
        return _summary.eventFlags != NoEvent;
    }

    EventSummary EventAccumulator::DumpAndReset()
    {
        EventSummary returnedSummary = _summary;
        _summary = EventSummary();
        return returnedSummary;
    }
}
