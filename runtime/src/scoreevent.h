#pragma once

#include "types.h"

namespace Dryad
{
    enum class ScoreEventType
    {
        PlayNote,
        TempoChange,
        GraphChange,
        ScaleChange,
    };

    struct ScoreNoteEvent
    {
        ScoreTime duration;
        NoteValue value;
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
        ScoreTime time;
        bool committed;
        ScoreEventType type;
        ScoreEventData data;

        inline const ScoreNoteEvent& GetNoteEvent() const
        {
            return data.Get<ScoreNoteEvent>();
        }

        inline const ScoreTempoEvent& GetTempoEvent() const
        {
            return data.Get<ScoreTempoEvent>();
        }

        inline const ScoreGraphEvent& GetGraphEvent() const
        {
            return data.Get<ScoreGraphEvent>();
        }

        inline const ScoreScaleEvent& GetScaleEvent() const
        {
            return data.Get<ScoreScaleEvent>();
        }
    };
}
