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
        Time timeOn;
        Time timeOff;
        NoteValue note;
        NoteVelocity velocity;
        const Motif* motif;
        const Graph* graph;
        const Scale* scale;
        const Node* node;
    };
}
