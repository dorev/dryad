#include "session.h"

namespace Dryad
{
    Result Session::Start(Time time)
    {
        score.Reset(time);
        return Success;
    }

    Result Session::PushEvent(Event& event)
    {
        switch(event.type)
        {
            case EventType::AddMotif:
            case EventType::RemoveMotif:
                if(event.data.Contains<Motif*>())
                {
                    return eventAccumulator.Consume(event.type, event.data.Get<Motif*>());
                }
                return InvalidEventData;

            case EventType::RequestInterlude:
            case EventType::CancelInterlude:
                if(event.data.Contains<Interlude*>())
                {
                    return eventAccumulator.Consume(event.type, event.data.Get<Interlude*>());
                }
                return InvalidEventData;

            case EventType::ChangeTempo:
                if(event.data.Contains<TempoChange>())
                {
                    return eventAccumulator.Consume(event.type, event.data.Get<TempoChange>());
                }
                return InvalidEventData;

            case EventType::ChangeScale:
            case EventType::ChangeGraph:
                if(event.data.Contains<HarmonicTransition>())
                {
                    return eventAccumulator.Consume(event.type, event.data.Get<HarmonicTransition>());
                }
                return InvalidEventData;

            default:
                return UnsupportedEventType;
        }
    }

    Result Session::Update(Time deltaTime, Vector<ScoreEvent>& output)
    {
        if(eventAccumulator.HasChanges())
        {
            EventSummary summary = eventAccumulator.DumpAndReset();

            




        }




        // check if the time progress leads to new notes



        return NotYetImplemented;
    }
}
