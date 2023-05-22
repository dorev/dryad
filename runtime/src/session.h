#pragma once

#include "types.h"
#include "containers.h"
#include "score.h"
#include "scoreevent.h"
#include "result.h"
#include "event.h"
#include "eventreducer.h"

namespace Dryad
{
    class Session
    {
    public:
        Result Start(Time time, Tempo tempo);
        Result PushEvent(Event& event);
        Result Update(Time deltaTime, Vector<ScoreEvent>& newCommittedEvents);

    private:
        Score score;
        EventReducer eventReducer;
    };
}
