#pragma once

#include "types.h"
#include "containers.h"
#include "math.h"
#include "scoreevent.h"
#include "scale.h"

namespace Dryad
{
    struct ScoreFrame
    {
        ScoreFrame()
            : startTime(0)
            , events()
            , next(nullptr)
            , prev(nullptr)
        {
        }

        ScoreTime startTime;
        List<ScoreEvent*> events;
        ScoreFrame* next;
        ScoreFrame* prev;
    };

    class ScoreLedger
    {
    public:
        ScoreLedger(Time startTime, Tempo startTempo, const Scale* startScale)
            : m_StartTime(startTime)
            , m_StartTempo(startTempo)
            , m_StartScale(startScale)
            , m_LastCommittedLedgerFrame(nullptr)
        {
        }

        NoteValue GetLastCommittedNoteValue() const
        {
            ScoreFrame* frame = m_LastCommittedLedgerFrame;
            while (frame != nullptr)
            {
                for (const ScoreEvent* event : frame->events)
                {
                    if (event->type == ScoreEventType::NotePlay)
                    {
                        return event->GetNoteEvent().value;
                    }
                }
                frame = frame->prev;
            }
            return MiddleC;
        }

        ScoreTime GetCommittedScoreDuration() const
        {
            ScoreFrame* frame = m_LastCommittedLedgerFrame;
            return frame != nullptr ? frame->startTime : 0;
        }

        ScoreFrame* GetFirstUncommittedFrame()
        {
            if (m_LastCommittedLedgerFrame == nullptr)
            {
                m_LastCommittedLedgerFrame = AppendNewFrame();
                return m_LastCommittedLedgerFrame;
            }
            else
            {
                return m_LastCommittedLedgerFrame->next;
            }
        }

        ScoreFrame* AppendNewFrame()
        {
            ScoreFrame* newFrame = new ScoreFrame();
            if (m_LedgerFrames.Size() > 0)
            {
                ScoreFrame* lastFrame = m_LedgerFrames.Back();
                lastFrame->next = newFrame;
                newFrame->prev = lastFrame;
                m_LedgerFrames.PushBack(newFrame);
            }
            else
            {
                m_LedgerFrames.PushBack(newFrame);
            }
            return newFrame;
        }

        const Time& GetStartTime() const
        {
            return m_StartTime;
        }

        const Tempo& GetStartTempo() const
        {
            return m_StartTempo;
        }

        const Scale* GetStartScale() const
        {
            return m_StartScale;
        }

    private:
        Time m_StartTime;
        Tempo m_StartTempo;
        const Scale* m_StartScale;
        ScoreFrame* m_LastCommittedLedgerFrame;
        List<ScoreFrame*> m_LedgerFrames;
    };
}
