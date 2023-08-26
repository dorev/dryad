#pragma once

#include "types.h"
#include "scoreevent.h"

namespace Dryad
{
    struct ScoreLedgerFrame
    {
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


    private:
        Time m_StartTime;
        Tempo m_StartTempo;
        const Scale* m_StartScale;
        ScoreLedgerFrame* m_LastCommittedLedgerFrame;
        List<ScoreLedgerFrame*> m_LedgerFrames;
    };
}
