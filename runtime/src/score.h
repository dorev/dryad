#pragma once

#include "types.h"
#include "time.h"
#include "note.h"
#include "event.h"
#include "scoreevent.h"
#include "result.h"
#include "harmonicframe.h"

namespace Dryad
{
    class Interlude;
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
        Position committedDuration;
        Int32 lastCommittedEventIndex;
    };

    class Score
    {
    public:
        Score();
        void Reset(Time startTime);
        Result UpdateHarmony(Interlude* interlude, HarmonicTransition& harmonicTransition);
        Result UpdateMotifs(Map<Motif*, Int32>& motifVariations);
        Result UpdateTempo(TempoChange& tempoChange);
        Result Commit(Time deltaTime, Vector<ScoreEvent>& newCommittedEvents);

    private:
        inline static const UInt32 InitialHarmonicFramesCount = 8;
        ScoreInfo _info;
        CircularDeque<HarmonicFrame> _harmonicFrames;
        Vector<ScoreEvent> _events;
    };
}
