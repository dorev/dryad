#pragma once

#include "types.h"
#include "scoreevent.h"

namespace Dryad
{
    class ScoreLedger
    {
    public:
        ScoreLedger(Time startTime, Tempo startTempo)
            : startTime(startTime)
            , startTempo(startTempo)
            , committedDuration()
            , lastCommittedEventIndex(-1)
        {
        }

        Time startTime;
        Tempo startTempo;
        ScoreTime committedDuration;
        Int32 lastCommittedEventIndex;
    };

}
