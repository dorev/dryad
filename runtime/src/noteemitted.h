#pragma once

#include "types.h"
#include "time.h"

namespace Dryad
{
    enum class ScoreEventType
    {
        NoteEmitted,
        TempoChange,
        GraphChange,
        ScaleChange,
    };

    struct ScoreEvent
    {
        NoteValue value;
        Time timeOn;
        Time timeOff;
        IdType motifId;
        void* motifUserData;
        void* graphUserData;
        void* nodeUserData;
    };
}
