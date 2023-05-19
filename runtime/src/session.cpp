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
            case AddMotif:
            case RemoveMotif:
                if(event.data.Contains<Motif*>())
                {
                    return eventAccumulator.Consume(event.type, event.data.Get<Motif*>());
                }
                return InvalidEventData;

            case RequestInterlude:
            case CancelInterlude:
                if(event.data.Contains<Interlude*>())
                {
                    return eventAccumulator.Consume(event.type, event.data.Get<Interlude*>());
                }
                return InvalidEventData;

            case ChangeTempo:
                if(event.data.Contains<TempoChange>())
                {
                    return eventAccumulator.Consume(event.type, event.data.Get<TempoChange>());
                }
                return InvalidEventData;

            case ChangeScale:
            case ChangeGraph:
                if(event.data.Contains<HarmonicTransition>())
                {
                    return eventAccumulator.Consume(event.type, event.data.Get<HarmonicTransition>());
                }
                return InvalidEventData;

            default:
                return UnsupportedEventType;
        }
    }

    Result Session::Update(Time deltaTime, Vector<NoteEmitted>& output)
    {
        if(eventAccumulator.HasChanges())
        {
            EventSummary summary = eventAccumulator.DumpAndReset();

            




        }




        // check if the time progress leads to new notes



        return NotYetImplemented;
    }
}
