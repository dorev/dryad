#pragma once

#include "types.h"
#include "note.h"
#include "event.h"
#include "scoreevent.h"
#include "scoreframe.h"
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
        void Reset(RealTime startTime, Tempo startTempo, const Scale* startScale);
        Result UpdateHarmony(HarmonyTransition& harmonyTransition);
        Result UpdateMotifs(Map<const Motif*, Int32>& motifsVariations);
        Result UpdateTempo(TempoChange& tempoChange);
        Result UpdateNotes(Map<const Motif*, Int32>& motifsVariations, const HarmonyTransition& harmonyTransition);
        Result Commit(RealTime deltaTime, Vector<ScoreEvent>& newCommittedEvents);
        Result GenerateHarmonyFrames(ScoreTime durationToAppend);
        Result GenerateHarmonyFramesUntil(ScoreTime targetScoreTIme);

        HarmonyFrame* CurrentHarmonyFrame();
        const HarmonyFrame* CurrentHarmonyFrame() const;
        HarmonyFrame* LastHarmonyFrame();
        const HarmonyFrame* LastHarmonyFrame() const;
        HarmonyFrame* HarmonyFrameBackwardSearch(const ScoreFrame* scoreFrame);
        const HarmonyFrame* HarmonyFrameBackwardSearch(const ScoreFrame* scoreFrame) const;
        Tempo CurrentTempo() const;
        const Scale* CurrentScale() const;
        const Node* CurrentNode();
        ScoreTime CurrentTime() const;
        const List<HarmonyFrame>& GetHarmonyFrames() const;
        NoteValue GetLastCommittedNoteValue() const;
        ScoreFrame* GetLastCommittedFrame();
        const ScoreFrame* GetLastCommittedFrame() const;
        ScoreFrame* GetFirstStagedFrame();
        ScoreFrame* AppendEmptyFrame();
        Result ClearHarmonyFramesAfter(HarmonyFrame* harmonyFrame);
        Result ClearHarmonyFramesAfter(ScoreTime time);
        Result InsertHarmonyFrame(HarmonyFrame* harmnyFrame);

        ScoreTime GeneratedEndTime() const;
        ScoreTime TimeRemainingToCurrentHarmonyFrame() const;
        NoteValue GetLatestOctaveRoot() const;

    private:
        inline static const UInt32 m_DefaultHarmonicFramesCount = 8;
        Map<const Motif*, UInt32> m_MotifLevels;
        Map<const Motif*, List<MotifInstance*>> m_MotifInstances;
        HarmonyStrategy m_HarmonyStrategy;
        MotifStrategy m_MotifStrategy;
        List<ScoreFrame*> m_CommittedScoreFrames;
        List<ScoreFrame*> m_StagedScoreFrames;

        RealTime m_StartTime;
        Tempo m_StartTempo;
        const Scale* m_StartScale;
    };
}
