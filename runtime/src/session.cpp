#include "session.h"
#include "scoreevent.h"
#include "flags.h"

namespace Dryad
{
    Result Session::Start(Time time, Tempo tempo, const Scale* scale)
    {
        // TODO: return an error if the score has already been started
        //       I might implement a "Pause" feature sometime...
        score.Reset(time, tempo, scale);
        return Result::Success;
    }

    Result Session::PushEvent(Event& event)
    {
        switch(event.type)
        {
            case EventType::AddMotif:
            case EventType::RemoveMotif:
                if(event.data.Contains<Motif*>())
                {
                    return eventReducer.Consume(event.type, event.data.Get<Motif*>());
                }
                return Result::InvalidEventData;

            case EventType::ChangeTempo:
                if(event.data.Contains<TempoChange>())
                {
                    return eventReducer.Consume(event.type, event.data.Get<TempoChange>());
                }
                return Result::InvalidEventData;

            case EventType::ChangeScale:
            case EventType::ChangeGraph:
                if(event.data.Contains<HarmonyTransition>())
                {
                    return eventReducer.Consume(event.type, event.data.Get<HarmonyTransition>());
                }
                return Result::InvalidEventData;

            default:
                return Result::UnsupportedEventType;
        }
    }

    Result Session::Update(Time deltaTime, Vector<ScoreEvent>& newCommittedEvents)
    {
        if(deltaTime <= 0)
        {
            return Result::UselessOperation;
        }
        Result result = Result::EmptyResult;
        if(eventReducer.HasChanges())
        {
            EventSummary summary = eventReducer.DumpAndReset();
            if(summary.HasHarmonicChanges())
            {
                result |= score.UpdateHarmony(summary.harmonicTransitionRequested);
            }
            if(summary.HasMotifChanges())
            {
                result |= result, score.UpdateMotifs(summary.motifVariations);
            }
            if(summary.HasTempoChanges())
            {
                result |= result, score.UpdateTempo(summary.tempoChangeRequested);
            }
        }
        if(result == Result::Success)
        {
            return result |= result, score.Commit(deltaTime, newCommittedEvents);
        }
        return result;
    }
}
