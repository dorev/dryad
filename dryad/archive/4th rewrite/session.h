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
        Result Start(RealTime time, Tempo tempo, const Scale* scale);
        Result PushEvent(Event& event);
        Result Update(RealTime deltaTime, Vector<ScoreEvent>& newCommittedEvents);

    private:
        Score m_Score;
        EventReducer m_EventReducer;
    };
}
