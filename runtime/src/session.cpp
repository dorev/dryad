#include "session.h"

namespace Dryad
{
    Result Session::Start(Time time)
    {
        score.Reset(time);
        return Success;
    }

    Result Session::PushEvent(const Event& event)
    {
        // Update eventAccumulator

        switch(event.type)
        {
            case AddMotif:
            case RemoveMotif:
                break;
            case RequestInterlude:
            case CancelInterlude:
            case ChangeTempo:
            case ChangeScale:
            case ChangeGraph:
                break;
        }

        return Success;
    }

    Result Session::Update(Time deltaTime, Vector<NoteEmitted>& output)
    {
        // Dequeue all event
        // Depending on their type update the current harmonic context
        

        return NotImplemented;
    }
}
