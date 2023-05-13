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
        eventQueue.push_back(event);
        return Success;
    }

    Result Session::Update(Time deltaTime, Vector<NoteEmitted>& output)
    {
        

        return NotImplemented;
    }
}