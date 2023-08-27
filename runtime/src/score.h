#pragma once

#include "types.h"
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
        Result UpdateHarmony(HarmonyTransition& harmonyTransition);
        Result UpdateMotifs(Map<const Motif*, Int32>& motifsVariations);
        Result UpdateTempo(TempoChange& tempoChange);
        Result UpdateNotes(Map<const Motif*, Int32>& motifsVariations, const HarmonyTransition& harmonyTransition);
        Result Commit(Time deltaTime, Vector<ScoreEvent>& newCommittedEvents);
        Result GenerateFrames(ScoreTime durationToAppend);
        Result GenerateFramesUntil(ScoreTime targetScoreTIme);

        Tempo CurrentTempo() const;
        const Scale* CurrentScale() const;
        const Node* CurrentNode();
        ScoreTime CurrentTime() const;
        HarmonyFrame& CurrentHarmonyFrame();
        const HarmonyFrame& CurrentHarmonyFrame() const;
        List<HarmonyFrame>& GetHarmonyFrames();
        const List<HarmonyFrame>& GetHarmonyFrames() const;
        ScoreTime GeneratedEndTime() const;
        ScoreTime TimeRemainingToCurrentHarmonyFrame() const;
        ScoreTime CurrentHarmonyFrameEndTime() const;
        NoteValue GetLatestOctaveRoot() const;

    private:
        inline static const UInt32 m_DefaultHarmonicFramesCount = 8;
        ScoreLedger m_Ledger;
        List<HarmonyFrame> m_HarmonyFrames;
        Map<const Motif*, UInt32> m_MotifLevels;
        Map<const Motif*, List<MotifInstance*>> m_MotifInstances;
        HarmonyStrategy m_HarmonyStrategy;
        MotifStrategy m_MotifStrategy;
    };
}
