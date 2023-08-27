#pragma once

#include "types.h"
#include "containers.h"
#include "math.h"
#include "scoreevent.h"
#include "scale.h"

namespace Dryad
{
    struct ScoreLedgerFrame
    {
        ScoreLedgerFrame()
            : time(0)
            , events()
            , next(nullptr)
            , prev(nullptr)
        {
        }

        ScoreTime time;
        List<ScoreEvent*> events;
        ScoreLedgerFrame* next;
        ScoreLedgerFrame* prev;
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
            ScoreLedgerFrame* frame = m_LastCommittedLedgerFrame;
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
            ScoreLedgerFrame* frame = m_LastCommittedLedgerFrame;
            return frame != nullptr ? frame->time : 0;
        }

        ScoreLedgerFrame* GetFirstUncommittedFrame()
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

        ScoreLedgerFrame* AppendNewFrame()
        {
            ScoreLedgerFrame* newFrame = new ScoreLedgerFrame();
            if (m_LedgerFrames.Size() > 0)
            {
                ScoreLedgerFrame* lastFrame = m_LedgerFrames.Back();
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
        ScoreLedgerFrame* m_LastCommittedLedgerFrame;
        List<ScoreLedgerFrame*> m_LedgerFrames;
    };
}
