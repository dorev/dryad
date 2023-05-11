#pragma once

#include "types.h"
#include "score.h"
#include "result.h"
#include "event.h"
#include "noteemitted.h"

namespace Dryad
{
    class Session
    {
    public:
        Result Start(Time time);
        Result Pause();
        Result PushEvent(const Event& event);
        Result Update(Time deltaTime, Vector<NoteEmitted>& output);

    private:
        Score score;
        Deque<HarmonicContext> harmonyQueue;
    };
}
