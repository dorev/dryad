#include "eventreducer.h"
#include "flags.h"

namespace Dryad
{
    Result EventReducer::Consume(EventType eventType, Motif* motif)
    {
        Int32* variationCount = nullptr;
        if (m_Summary.motifVariations.Find(motif, variationCount))
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
            if ((*variationCount) == 0 && !m_Summary.motifVariations.Remove(motif))
            {
                return Result::PotentialConcurrencyError;
            }
        }
        else
        {
            switch(eventType)
            {
                case EventType::AddMotif:
                    m_Summary.motifVariations[motif] = 1;
                    break;
                case EventType::RemoveMotif:
                    break;
                default:
                    return Result::InvalidEventType;
            }
        }
        SetFlag(m_Summary.eventFlags, eventType);
        return Result::Success;
    }

    Result EventReducer::Consume(EventType eventType, TempoChange tempoChange)
    {
        if (eventType == EventType::ChangeTempo)
        {
            m_Summary.tempoChangeRequested = tempoChange;
            SetFlag(m_Summary.eventFlags, eventType);
            return Result::Success;
        }
        return Result::InvalidEventType;
    }

    Result EventReducer::Consume(EventType eventType, HarmonyTransition harmonicTransition)
    {
        switch(eventType)
        {
            case EventType::ChangeScale:
            case EventType::ChangeGraph:
                m_Summary.harmonyTransitionRequested.Merge(harmonicTransition);
                break;
            default:
                return Result::InvalidEventType;
        }
        SetFlag(m_Summary.eventFlags, eventType);
        return Result::Success;
    }

    bool EventReducer::HasChanges()
    {
        return m_Summary.eventFlags != EventType::NoEvent;
    }

    EventSummary EventReducer::DumpAndReset()
    {
        EventSummary returnedSummary = m_Summary;
        m_Summary = EventSummary();
        return returnedSummary;
    }
}
