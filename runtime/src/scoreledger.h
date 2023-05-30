#pragma once

#include "types.h"
#include "scoreevent.h"

namespace Dryad
{
    class ScoreLedger
    {
    public:
        ScoreLedger(Time startTime, Tempo startTempo, const Scale* startScale)
            : startTime(startTime)
            , startTempo(startTempo)
            , startScale(startScale)
            , committedDuration()
            , lastCommittedEventIndex(-1)
        {
        }

        Time startTime;
        Tempo startTempo;
        const Scale* startScale;
        ScoreTime committedDuration;
        Int32 lastCommittedEventIndex;
    };

}
