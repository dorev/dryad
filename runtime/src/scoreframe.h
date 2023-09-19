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
        ScoreFrame()
            : startTime(0)
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
            return nullptr;
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

        // Still need to use these 2
        HarmonyFrame* harmonyFrame;
        bool committed;
    };
}
