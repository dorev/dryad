#pragma once

#include "types.h"

namespace Dryad
{

    enum class ScoreEventType
    {
        NoteOn,
        NoteOff
    };

    struct ScoreEvent
    {
        ScoreEventType type;
        Time position;
        NoteValue noteValue;
        Time noteDuration;
        int voiceId;
    };

} // namespace Dryad
