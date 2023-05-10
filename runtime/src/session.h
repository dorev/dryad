#pragma once

#include "types.h"
#include "identification.h"
#include "score.h"
#include "result.h"
#include "event.h"
#include "noteemitted.h"

namespace Dryad
{
    class Session : public Named
    {
        Score score;
        Deque<HarmonicContext> harmonyQueue;

        Result Start(Time time);
        Result Pause();
        Result PushEvent(const Event&);
        Result Commit(Time, Vector<NoteEmitted>&);

        Time PositionToTime(Position);
    };
}
