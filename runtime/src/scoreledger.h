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


        // How to store the ScoreEvent in the ledger?
        // multimap using time as key?
        


        Time startTime;
        Tempo startTempo;
        const Scale* startScale;
        ScoreTime committedDuration;
        Int32 lastCommittedEventIndex;
    };

}
