#include "session.h"
#include "scoreevent.h"
#include "flags.h"

namespace Dryad
{
    Result Session::Start(Time time, Tempo tempo, const Scale* scale)
    {
        // TODO: return an error if the score has already been started
        //       I might implement a "Pause" feature sometime...
        m_Score.Reset(time, tempo, scale);
        return Result::Success;
    }

    Result Session::PushEvent(Event& event)
    {
        switch(event.type)
        {
            case EventType::AddMotif:
            case EventType::RemoveMotif:
                if (event.data.Contains<Motif*>())
                {
                    return m_EventReducer.Consume(event.type, event.data.Get<Motif*>());
                }
                return Result::InvalidEventData;

            case EventType::ChangeTempo:
                if (event.data.Contains<TempoChange>())
                {
                    return m_EventReducer.Consume(event.type, event.data.Get<TempoChange>());
                }
                return Result::InvalidEventData;

            case EventType::ChangeScale:
            case EventType::ChangeGraph:
                if (event.data.Contains<HarmonyTransition>())
                {
                    return m_EventReducer.Consume(event.type, event.data.Get<HarmonyTransition>());
                }
                return Result::InvalidEventData;

            default:
                return Result::UnsupportedEventType;
        }
    }

    Result Session::Update(Time deltaTime, Vector<ScoreEvent>& newCommittedEvents)
    {
        if (deltaTime == 0)
        {
            return Result::UselessOperation;
        }
        if (deltaTime < 0)
        {
            return Result::InvalidOperation;
        }
        Result result = Result::EmptyResult;
        if (m_EventReducer.HasChanges())
        {
            EventSummary summary = m_EventReducer.DumpAndReset();
            bool harmonyChanged = summary.HasHarmonyChanges();
            bool motifsChanged = summary.HasMotifChanges();
            bool tempoChanged = summary.HasTempoChanges();
            if (harmonyChanged)
            {
                result |= m_Score.UpdateHarmony(summary.harmonyTransitionRequested);
            }
            if (motifsChanged)
            {
                result |= m_Score.UpdateMotifs(summary.motifVariations);
            }
            if (harmonyChanged || motifsChanged)
            {
                result |= m_Score.UpdateNotes(motifsChanged, harmonyChanged);
            }
            if (tempoChanged)
            {
                result |= m_Score.UpdateTempo(summary.tempoChangeRequested);
            }
        }
        if (result == Result::Success)
        {
            return result |= result, m_Score.Commit(deltaTime, newCommittedEvents);
        }
        return result;
    }
}
