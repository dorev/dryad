// This file contains all the Score functions that do not implement functional logic,
// but rather 'accessory' functions that supports the functional aspect, such as lookup
// or simple informative functions.

#include "score.h"
#include "graph.h"
#include "edge.h"
#include "node.h"
#include "random.h"
#include "scale.h"
#include "motif.h"
#include "motifinstance.h"

namespace Dryad
{
    void Score::Reset(Time startTime, Tempo startTempo, const Scale* startScale)
    {
        m_Ledger = ScoreLedger(startTime, startTempo, startScale);
        //_events.Clean(); TODO: Clean ledger events
        m_HarmonyFrames.Clear();
    }

    HarmonyFrame& Score::CurrentHarmonyFrame()
    {
        return m_HarmonyFrames.Front();
    }

    const HarmonyFrame& Score::CurrentHarmonyFrame() const
    {
        return m_HarmonyFrames.Front();
    }

    List<HarmonyFrame>& Score::GetHarmonyFrames()
    {
        return m_HarmonyFrames;
    }

    const List<HarmonyFrame>& Score::GetHarmonyFrames() const
    {
        return m_HarmonyFrames;
    }

    Tempo Score::CurrentTempo() const
    {
        if (m_HarmonyFrames.Empty())
        {
            return m_Ledger.GetStartTempo();
        }
        return CurrentHarmonyFrame().tempo;
    }

    const Scale* Score::CurrentScale() const
    {
        if (m_HarmonyFrames.Empty())
        {
            // TODO: return the default scale?
            return m_Ledger.GetStartScale();
        }
        return CurrentHarmonyFrame().scale;
    }

    const Node* Score::CurrentNode()
    {
        if (m_HarmonyFrames.Empty())
        {
            return nullptr;
        }
        return CurrentHarmonyFrame().node;
    }

    ScoreTime Score::CurrentTime() const
    {
        return m_Ledger.GetCommittedScoreDuration();
    }

    ScoreTime Score::TimeRemainingToCurrentHarmonyFrame() const
    {
        if (m_HarmonyFrames.Empty())
        {
            return 0;
        }
        return CurrentHarmonyFrameEndTime() - CurrentTime();
    }

    ScoreTime Score::CurrentHarmonyFrameEndTime() const
    {
        if (m_HarmonyFrames.Empty())
        {
            return 0;
        }
        return CurrentHarmonyFrame().EndTime();
    }

    NoteValue Score::GetLatestOctaveRoot() const
    {
        if (m_HarmonyFrames.Empty())
        {
            const Scale* scale = CurrentScale();
            return MiddleC + scale->root;
        }
        else
        {
            NoteValue root = m_HarmonyFrames.Back().scale->root;
            NoteValue reference = m_Ledger.GetLastCommittedNoteValue() % Notes[C][1];
            return reference + root;
        }
    }

    ScoreTime Score::GeneratedEndTime() const
    {
        if (m_HarmonyFrames.Empty())
        {
            return 0;
        }
        return m_HarmonyFrames.Back().EndTime();
    }

    Result Score::GenerateFramesUntil(ScoreTime targetScoreTime)
    {
        ScoreTime currentScoreEnd = GeneratedEndTime();
        if (targetScoreTime > currentScoreEnd)
        {
            return GenerateFrames(targetScoreTime - currentScoreEnd);
        }
        return Result::Success;
    }
}
