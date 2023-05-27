#pragma once

#include "types.h"
#include "time.h"
#include "note.h"
#include "event.h"
#include "scoreevent.h"
#include "result.h"
#include "harmonyframe.h"
#include "harmonystrategy.h"
#include "motifstrategy.h"

namespace Dryad
{
    class Motif;
    class Session;

    struct ScoreInfo
    {
        ScoreInfo(Time startTime)
            : startTime(startTime)
            , committedDuration()
            , lastCommittedEventIndex(-1)
        {
        }

        Time startTime;
        ScoreTime committedDuration;
        Int32 lastCommittedEventIndex;
    };

    class Score
    {
    public:
        Score();
        void Reset(Time startTime);
        Result UpdateHarmony(HarmonyTransition& harmonicTransition);
        Result UpdateMotifs(Map<Motif*, Int32>& motifVariations);
        Result UpdateTempo(TempoChange& tempoChange);
        Result Commit(Time deltaTime, Vector<ScoreEvent>& newCommittedEvents);

    private:
        inline static const UInt32 DefaultHarmonicFramesCount = 8;
        ScoreInfo _info;
        Deque<HarmonyFrame> _harmonyFrames;
        Vector<ScoreEvent> _events;
        HarmonyStrategy _harmonyStrategy;
        MotifStrategy _motifStrategy;
    };
}
