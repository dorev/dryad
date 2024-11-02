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
        NoteValue GetLastCommittedNoteValue() const;
        ScoreFrame*& GetLastCommittedFrame();
        ScoreFrame*& GetLastStagedFrame();
        const ScoreFrame* GetLastStagedFrame() const;
        const ScoreFrame* GetLastCommittedFrame() const;
        ScoreFrame*& GetFirstStagedFrame();
        ScoreFrame*& GetFirstCommittedFrame();
        Result ClearHarmonyFramesAfter(HarmonyFrame* harmonyFrame);
        Result ClearHarmonyFramesAfter(ScoreTime time);
        Result InsertHarmonyFrame(HarmonyFrame* harmonyFrame);
        ScoreFrame* GetOrCreateScoreFrame(ScoreTime startTime);

        ScoreTime GeneratedEndTime() const;
        ScoreTime TimeRemainingToCurrentHarmonyFrame() const;
        NoteValue GetLatestOctaveRoot() const;
        Result AddMotif(const Motif* motif, UInt32 count, ScoreFrame* scoreFrame);

    private:
        inline static const UInt32 m_DefaultHarmonicFramesCount = 8;
        Map<const Motif*, UInt32> m_MotifLevels;
        Map<const Motif*, List<MotifInstance*>> m_MotifInstances;
        HarmonyStrategy m_HarmonyStrategy;
        MotifStrategy m_MotifStrategy;

        ScoreFrame* m_FirstCommittedScoreFrame;
        ScoreFrame* m_LastCommittedScoreFrame;
        ScoreFrame* m_FirstStagedScoreFrame;
        ScoreFrame* m_LastStagedScoreFrame;

        RealTime m_StartTime;
        Tempo m_StartTempo;
        const Scale* m_StartScale;
    };
}
