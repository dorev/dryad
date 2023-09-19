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
    void Score::Reset(RealTime startTime, Tempo startTempo, const Scale* startScale)
    {
        m_StartTime = startTime;
        m_StartTempo = startTempo;
        m_StartScale = startScale;
        m_CommittedScoreFrames.Clear();
        m_StagedScoreFrames.Clear();
    }

    HarmonyFrame* Score::CurrentHarmonyFrame()
    {
        return HarmonyFrameBackwardSearch(GetLastCommittedFrame());
    }

    const HarmonyFrame* Score::CurrentHarmonyFrame() const
    {
        return HarmonyFrameBackwardSearch(GetLastCommittedFrame());
    }

    HarmonyFrame* Score::LastHarmonyFrame()
    {
        return HarmonyFrameBackwardSearch(m_StagedScoreFrames.Back());
    }

    const HarmonyFrame* Score::LastHarmonyFrame() const
    {
        return HarmonyFrameBackwardSearch(m_StagedScoreFrames.Back());
    }

    HarmonyFrame* Score::HarmonyFrameBackwardSearch(const ScoreFrame* scoreFrame)
    {
        HarmonyFrame* harmonyFrame = nullptr;
        while (scoreFrame != nullptr)
        {
            for (ScoreEvent* event : scoreFrame->scoreEvents)
            {
                if (event != nullptr && event->type == ScoreEventType::HarmonyFrameChange)
                {
                    return event->GetHarmonyFrameData();
                }
            }
            scoreFrame = scoreFrame->prev;
        }
        return harmonyFrame;
    }

    const HarmonyFrame* Score::HarmonyFrameBackwardSearch(const ScoreFrame* scoreFrame) const
    {
        const HarmonyFrame* harmonyFrame = nullptr;
        while (scoreFrame != nullptr)
        {
            for (const ScoreEvent* event : scoreFrame->scoreEvents)
            {
                if (event != nullptr && event->type == ScoreEventType::HarmonyFrameChange)
                {
                    return event->GetHarmonyFrameData();
                }
            }
            scoreFrame = scoreFrame->prev;
        }
        return harmonyFrame;
    }

    ScoreFrame* Score::GetFirstStagedFrame()
    {
        if (m_StagedScoreFrames.Empty())
        {
            ScoreFrame* newFrame = new ScoreFrame();
            m_StagedScoreFrames.PushBack(newFrame);
        }
        return m_StagedScoreFrames.Front();
    }

    ScoreFrame* Score::GetLastCommittedFrame()
    {
        if (m_CommittedScoreFrames.Empty())
        {
            return nullptr;
        }
        return m_CommittedScoreFrames.Back();
    }

    const ScoreFrame* Score::GetLastCommittedFrame() const
    {
        if (m_CommittedScoreFrames.Empty())
        {
            return nullptr;
        }
        return m_CommittedScoreFrames.Back();
    }

    Tempo Score::CurrentTempo() const
    {
        const HarmonyFrame* harmonyFrame = CurrentHarmonyFrame();
        if (harmonyFrame == nullptr)
        {
            return DefaultTempo;
        }
        return harmonyFrame->tempo;
    }

    const Scale* Score::CurrentScale() const
    {
        const HarmonyFrame* harmonyFrame = CurrentHarmonyFrame();
        if (harmonyFrame == nullptr)
        {
            return &MajorScale;
        }
        return harmonyFrame->scale;
    }

    const Node* Score::CurrentNode()
    {
        const HarmonyFrame* harmonyFrame = CurrentHarmonyFrame();
        if (harmonyFrame == nullptr)
        {
            return nullptr;
        }
        return harmonyFrame->node;
    }

    ScoreTime Score::CurrentTime() const
    {
        if (m_CommittedScoreFrames.Empty())
        {
            return 0;
        }
        return m_CommittedScoreFrames.Back()->startTime;
    }

    ScoreTime Score::TimeRemainingToCurrentHarmonyFrame() const
    {
        const HarmonyFrame* harmonyFrame = CurrentHarmonyFrame();
        if (harmonyFrame == nullptr)
        {
            return 0;
        }
        return harmonyFrame->EndTime() - CurrentTime();
    }

    NoteValue Score::GetLastCommittedNoteValue() const
    {
        if (m_CommittedScoreFrames.NotEmpty())
        {
            // Walk back through the frames from the last one and return
            // the first note event found
            ScoreFrame* frame = m_CommittedScoreFrames.Back();
            while (frame != nullptr)
            {
                for (const ScoreEvent* event : frame->scoreEvents)
                {
                    if (event->type == ScoreEventType::PlayNote)
                    {
                        return event->GetNoteData().value;
                    }
                }
                frame = frame->prev;
            }
        }
        return MiddleC;
    }

    NoteValue Score::GetLatestOctaveRoot() const
    {
        const HarmonyFrame* harmonyFrame = CurrentHarmonyFrame();
        if (harmonyFrame == nullptr)
        {
            const Scale* scale = CurrentScale();
            return MiddleC + scale->root;
        }
        else
        {
            NoteValue root = harmonyFrame->scale->root;
            NoteValue reference = GetLastCommittedNoteValue() % Notes[C][1];
            return reference + root;
        }
    }

    ScoreTime Score::GeneratedEndTime() const
    {
        const HarmonyFrame* harmonyFrame = CurrentHarmonyFrame();
        if (harmonyFrame == nullptr)
        {
            return 0;
        }
        return harmonyFrame->EndTime();
    }

    Result Score::GenerateHarmonyFramesUntil(ScoreTime targetScoreTime)
    {
        ScoreTime currentScoreEnd = GeneratedEndTime();
        if (targetScoreTime > currentScoreEnd)
        {
            return GenerateHarmonyFrames(targetScoreTime - currentScoreEnd);
        }
        return Result::Success;
    }

    Result Score::ClearHarmonyFramesAfter(HarmonyFrame* harmonyFrame)
    {
        if (harmonyFrame == nullptr)
        {
            return Result::UselessOperation;
        }
        ScoreFrame* scoreFrame = harmonyFrame->scoreFrame;
        if (scoreFrame == nullptr)
        {
            return Result::InvalidHarmonyFrame;
        }
        harmonyFrame->next = nullptr;

        // Find all the following score frames with harmony frame events and delete
        // the harmony event
        scoreFrame = scoreFrame->next;
        while (scoreFrame != nullptr)
        {
            harmonyFrame = scoreFrame->GetHarmonyFrame();
            if (harmonyFrame != nullptr)
            {
                scoreFrame->DeleteHarmonyFrame();
            }
            scoreFrame = scoreFrame->next;
        }
        return Result::Success;
    }

    Result Score::ClearHarmonyFramesAfter(ScoreTime time)
    {
        // We start at the latest committed harmony frame because we cannot clear frames from the past
        HarmonyFrame* harmonyFrame = CurrentHarmonyFrame();
        if (harmonyFrame == nullptr)
        {
            return Result::UselessOperation;
        }

        // Find the latest frame possible
        while (harmonyFrame->next != nullptr && harmonyFrame->next->startTime < time)
        {
            harmonyFrame = harmonyFrame->next;
        }
        return ClearHarmonyFramesAfter(harmonyFrame);
    }

    Result Score::InsertHarmonyFrame(HarmonyFrame* harmonyFrame)
    {
        if (harmonyFrame == nullptr)
        {
            return Result::UselessOperation;
        }
        return Result::NotYetImplemented;
    }
}
