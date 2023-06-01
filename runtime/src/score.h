#pragma once

#include "types.h"
#include "scoretime.h"
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
        void Reset(Time startTime, Tempo startTempo, const Scale* startScale);
        Result UpdateHarmony(HarmonyTransition& harmonicTransition);
        Result UpdateMotifs(Map<Motif*, Int32>& motifVariations);
        Result UpdateTempo(TempoChange& tempoChange);
        Result Commit(Time deltaTime, Vector<ScoreEvent>& newCommittedEvents);

        Tempo CurrentTempo() const;
        const Scale* CurrentScale() const;
        Node* CurrentNode();
        ScoreTime CurrentTime() const;
        HarmonyFrame& CurrentHarmonyFrame();
        const HarmonyFrame& CurrentHarmonyFrame() const;
        Deque<HarmonyFrame>& GetHarmonyFrames();
        const Deque<HarmonyFrame>& GetHarmonyFrames() const;
        ScoreTime TimeRemainingToCurrentFrame() const;

    private:
        inline static const UInt32 DefaultHarmonicFramesCount = 8;
        ScoreLedger _ledger;
        Deque<HarmonyFrame> _harmonyFrames;
        HarmonyStrategy _harmonyStrategy;
        MotifStrategy _motifStrategy;
    };
}
