#pragma once

#include "types.h"
#include "containers.h"
#include "math.h"
#include "scoreevent.h"
#include "scale.h"
#include "memory.h"

namespace Dryad
{
    class HarmonyFrame;

    class ScoreFrame
    {
    public:
        ScoreFrame(ScoreTime scoreTime = 0)
            : startTime(scoreTime)
            , scoreEvents()
            , next(nullptr)
            , prev(nullptr)
        {
        }

        ScoreTime startTime;
        List<ScoreEvent*> scoreEvents;
        ScoreFrame* next;
        ScoreFrame* prev;

        inline HarmonyFrame* GetHarmonyFrame()
        {
            for(ScoreEvent* event : scoreEvents)
            {
                if (event->type == ScoreEventType::HarmonyFrameChange)
                {
                    return event->GetHarmonyFrameData();
                }
            }
            ScoreFrame* scoreFrame = this;
            while (scoreFrame != nullptr)
            {
                HarmonyFrame* harmonyFrame = scoreFrame->HasHarmonyChanges();
                if (harmonyFrame != nullptr)
                {
                    return harmonyFrame;
                }
                scoreFrame = prev;
            }
            return nullptr;
        }

        Result AddHarmonyFrame(HarmonyFrame* harmonyFrame)
        {
            // Check if there is not already a harmony frame or graph or scale change
            if (harmonyFrame != nullptr)
            {
                if (HasHarmonyChanges())
                {
                    return Result::ScoreFrameAlreadyContainsHarmonyChange;
                }
                ScoreEvent* scoreEvent = new ScoreEvent();
                scoreEvent->SetHarmonyFrame(harmonyFrame);
                scoreEvents.PushBack(scoreEvent);
                return Result::Success;
            }
            return Result::InvalidHarmonyFrame;
        }

        Result AddNote(const Note& note)
        {
            ScoreNoteEvent noteEvent = ScoreNoteEvent(note);
            ScoreEvent* scoreEvent = new ScoreEvent(ScoreNoteEvent(note), note.startTime);
            return Add(scoreEvent);
        }

        HarmonyFrame* HasHarmonyChanges()
        {
            for (ScoreEvent* scoreEvent : scoreEvents)
            {
                if (scoreEvent != nullptr && scoreEvent->IsHarmonyFrame())
                {
                    scoreEvent->GetHarmonyFrameData();
                }
            }
            return nullptr;
        }

        Result Add(ScoreEvent* scoreEvent)
        {
            // Check if there is not already a harmony frame or graph or scale change
            if (scoreEvent->IsHarmonyFrame())
            {
                return Result::ScoreFrameAlreadyContainsHarmonyChange;
            }
            else if (scoreEvent->IsGraphChange())
            {
                //DeleteGraphChange();
            }
            else if (scoreEvent->IsScaleChange())
            {
                //DeleteScaleChange();
            }
            else if (scoreEvent->IsTempoChange())
            {
                //DeleteTempoChange();
            }

            scoreEvents.PushBack(scoreEvent);
            return Result::Success;
        }

        void DeleteHarmonyFrame()
        {
            for (auto it = scoreEvents.begin(); it != scoreEvents.end(); it++)
            {
                ScoreEvent* scoreEvent = *it;
                if (scoreEvent != nullptr && scoreEvent->IsHarmonyFrame())
                {
                    SafeDelete(scoreEvent->GetHarmonyFrameData());
                    scoreEvents.Remove(scoreEvent);
                }
            }
        }

        void InsertBefore(ScoreFrame* other)
        {
            if (other != nullptr)
            {
                next = other;
                prev = other->prev;
                other->prev = this;
            }
        }

        void InsertAfter(ScoreFrame* other)
        {
            if (other != nullptr)
            {
                prev = other;
                next = other->next;
                other->next = this;
            }
        }
        bool committed;
    };
}
