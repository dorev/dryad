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

        // Delete all the committed score frames
        ScoreFrame* scoreFrameToDelete = GetFirstCommittedFrame();
        ScoreFrame* nextScoreFrameToDelete = nullptr;
        while(scoreFrameToDelete != nullptr)
        {
            nextScoreFrameToDelete = scoreFrameToDelete->next;
            SafeDelete(scoreFrameToDelete);
            scoreFrameToDelete = nextScoreFrameToDelete;
        }
        m_LastCommittedScoreFrame = nullptr;

        // Delete all the staged score frames
        scoreFrameToDelete = GetFirstStagedFrame();
        while(scoreFrameToDelete != nullptr)
        {
            nextScoreFrameToDelete = scoreFrameToDelete->next;
            SafeDelete(scoreFrameToDelete);
            scoreFrameToDelete = nextScoreFrameToDelete;
        }
        m_LastStagedScoreFrame = nullptr;
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
        return HarmonyFrameBackwardSearch(GetLastStagedFrame());
    }

    const HarmonyFrame* Score::LastHarmonyFrame() const
    {
        return HarmonyFrameBackwardSearch(GetLastStagedFrame());
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

    ScoreFrame*& Score::GetFirstStagedFrame()
    {
        return m_FirstStagedScoreFrame;
    }

    ScoreFrame*& Score::GetFirstCommittedFrame()
    {
        return m_FirstCommittedScoreFrame;
    }

    ScoreFrame*& Score::GetLastCommittedFrame()
    {
        return m_LastCommittedScoreFrame;
    }

    ScoreFrame*& Score::GetLastStagedFrame()
    {
        return m_LastStagedScoreFrame;
    }

    const ScoreFrame* Score::GetLastStagedFrame() const
    {
        return m_LastStagedScoreFrame;
    }

    const ScoreFrame* Score::GetLastCommittedFrame() const
    {
        return m_LastCommittedScoreFrame;
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
        const ScoreFrame* scoreFrame = GetLastCommittedFrame();
        return scoreFrame == nullptr ? 0 : scoreFrame->startTime;
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
        // Walk back through the frames from the last one and return the first note event found
        const ScoreFrame* scoreFrame = GetLastCommittedFrame();
        while (scoreFrame != nullptr)
        {
            for (const ScoreEvent* event : scoreFrame->scoreEvents)
            {
                if (event->type == ScoreEventType::PlayNote)
                {
                    return event->GetNoteData().value;
                }
            }
            scoreFrame = scoreFrame->prev;
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
            LOG_WARN("Cannot clear harmony frames after score time %d because there are no harmony frames", time);
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
            LOG_WARN("Cannot insert null harmony frame");
            return Result::UselessOperation;
        }

        if (CurrentTime() >= harmonyFrame->startTime)
        {
            LOG_WARN("Cannot insert harmony frame into a time slot that's already committed.");
            return Result::OperationFailed;
        }

        // First, check if we can insert it into the staged frames.
        ScoreFrame* scoreFrame = GetFirstStagedFrame();
        while (scoreFrame != nullptr)
        {
            if (scoreFrame->startTime == harmonyFrame->startTime)
            {
                if (scoreFrame->HasHarmonyChanges())
                {
                    LOG_WARN("A ScoreFrame with matching start time already has a HarmonyFrame.");
                    return Result::OperationFailed;
                }

                scoreFrame->AddHarmonyFrame(harmonyFrame);
                return Result::Success;
            }
            else if (scoreFrame->startTime > harmonyFrame->startTime)
            {
                // Insert the HarmonyFrame before the current ScoreFrame in a new ScoreFrame.
                
                ScoreFrame* newFrame = new ScoreFrame(harmonyFrame->startTime);
                newFrame->AddHarmonyFrame(harmonyFrame);
                newFrame->InsertBefore(scoreFrame);
                return Result::Success;
            }
        }

        // If we reach here, the HarmonyFrame's startTime is after all the staged ScoreFrames.
        // Add it to the end.
        ScoreFrame* newFrame = new ScoreFrame(harmonyFrame->startTime);
        newFrame->AddHarmonyFrame(harmonyFrame);
        scoreFrame = GetLastStagedFrame();
        if (scoreFrame = nullptr)
        {
            GetLastStagedFrame() = newFrame;
            GetFirstStagedFrame() = newFrame;
        }
        else
        {
            newFrame->InsertAfter(scoreFrame);
        }

        return Result::Success;
    }
}
