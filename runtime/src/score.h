#pragma once

#include "types.h"
#include "time.h"
#include "note.h"
#include "event.h"
#include "scoreevent.h"
#include "scoreledger.h"
#include "result.h"
#include "harmonicframe.h"

namespace Dryad
{
    class Motif;
    class Session;
    
    class Score
    {
    public:
        Score();
        void Reset(Time startTime, Tempo startTempo);
        Result UpdateHarmony(HarmonicTransition& harmonicTransition);
        Result UpdateMotifs(Map<Motif*, Int32>& motifVariations);
        Result UpdateTempo(TempoChange& tempoChange);
        Result Commit(Time deltaTime, Vector<ScoreEvent>& newCommittedEvents);

        Tempo CurrentTempo() const;
        Scale* CurrentScale() const;
        Node* CurrentNode();
        HarmonicFrame& CurrentHarmonicFrame();
        const HarmonicFrame& CurrentHarmonicFrame() const;

    private:
        inline static const UInt32 DefaultHarmonicFramesCount = 8;
        ScoreLedger _ledger;
        CircularDeque<HarmonicFrame> _harmonicFrames;

    private:
        Result InitialHarmonicFrame(HarmonicTransition& transition);
        Result UpdateHarmonyScale(HarmonicTransition& transition);
        Result UpdateHarmonyGraph(HarmonicTransition& transition);
    };
}
