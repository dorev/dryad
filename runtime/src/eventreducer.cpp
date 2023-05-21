#include "eventreducer.h"
#include "flags.h"

namespace Dryad
{
    Result EventReducer::Consume(EventType eventType, Motif* motif)
    {
        Int32* variationCount = nullptr;
        if(_summary.motifVariations.Find(motif, variationCount))
        {
            switch(eventType)
            {
                case EventType::AddMotif:
                    (*variationCount)++;
                    break;
                case EventType::RemoveMotif:
                    (*variationCount)--;
                    break;
                default:
                    return Result::InvalidEventType;
            }
            if((*variationCount) == 0 && !_summary.motifVariations.Remove(motif))
            {
                return Result::ConcurrencyError;
            }
        }
        else
        {
            switch(eventType)
            {
                case EventType::AddMotif:
                    _summary.motifVariations[motif] = 1;
                    break;
                case EventType::RemoveMotif:
                    break;
                default:
                    return Result::InvalidEventType;
            }
        }
        SetFlag(_summary.eventFlags, eventType);
        return Result::Success;
    }

    Result EventReducer::Consume(EventType eventType, TempoChange tempoChange)
    {
        if(eventType == EventType::ChangeTempo)
        {
            _summary.tempoChangeRequested = tempoChange;
            SetFlag(_summary.eventFlags, eventType);
            return Result::Success;
        }
        return Result::InvalidEventType;
    }

    Result EventReducer::Consume(EventType eventType, HarmonicTransition harmonicTransition)
    {
        switch(eventType)
        {
            case EventType::ChangeScale:
            case EventType::ChangeGraph:
                _summary.harmonicTransitionRequested.Merge(harmonicTransition);
                break;
            default:
                return Result::InvalidEventType;
        }
        SetFlag(_summary.eventFlags, eventType);
        return Result::Success;
    }

    bool EventReducer::HasChanges()
    {
        return _summary.eventFlags != EventType::NoEvent;
    }

    EventSummary EventReducer::DumpAndReset()
    {
        EventSummary returnedSummary = _summary;
        _summary = EventSummary();
        return returnedSummary;
    }
}
