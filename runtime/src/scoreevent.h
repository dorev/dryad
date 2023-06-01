#pragma once

#include "types.h"

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
        ScoreEventType type;
        bool committed;
        NoteValue note;
        NoteVelocity velocity;
        Time timeOn;
        Time timeOff;
        const Motif* motif;
        const Graph* graph;
        const Node* node;
    };
}
