#pragma once

#include "types.h"
#include "time.h"
#include "note.h"
#include "event.h"
#include "scoreevent.h"
#include "scoreledger.h"
#include "result.h"
#include "harmonyframe.h"
#include "harmonystrategy.h"
#include "motifstrategy.h"

namespace Dryad
{
    class Motif;
    class Session;
    
    class Score
    {
    public:
        Score();
        void Reset(Time startTime, Tempo startTempo);
        Result UpdateHarmony(HarmonyTransition& harmonicTransition);
        Result UpdateMotifs(Map<Motif*, Int32>& motifVariations);
        Result UpdateTempo(TempoChange& tempoChange);
        Result Commit(Time deltaTime, Vector<ScoreEvent>& newCommittedEvents);

        Tempo CurrentTempo() const;
        Scale* CurrentScale() const;
        Node* CurrentNode();
        HarmonyFrame& CurrentHarmonicFrame();
        const HarmonyFrame& CurrentHarmonicFrame() const;

    private:
        inline static const UInt32 DefaultHarmonicFramesCount = 8;
        ScoreLedger _ledger;
        Deque<HarmonyFrame> _harmonyFrames;
        HarmonyStrategy _harmonyStrategy;
        MotifStrategy _motifStrategy;
    };
}
