#pragma once

#include "types.h"

namespace Dryad
{
    enum class ScoreEventType
    {
        NotePlay,
        TempoChange,
        GraphChange,
        ScaleChange,
    };

    struct ScoreNoteEvent
    {
        Time duration;
        NoteValue note;
        NoteVelocity velocity;
        const Motif* motif;
        const Scale* scale;
        const Node* node;
    };

    struct ScoreTempoEvent
    {
        Tempo tempo;
    };

    struct ScoreGraphEvent
    {
        const Graph* graph;
    };

    struct ScoreScaleEvent
    {
        const Scale* scale;
    };

    using ScoreEventData = Variant<ScoreNoteEvent, ScoreTempoEvent, ScoreGraphEvent, ScoreScaleEvent>;

    struct ScoreEvent
    {
        Time time;
        bool committed;
        ScoreEventType type;
        ScoreEventData data;
    };
}
