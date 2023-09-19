#pragma once

#include "types.h"

namespace Dryad
{
    class Graph;
    class Scale;
    class HarmonyFrame;

    enum class ScoreEventType
    {
        PlayNote,
        TempoChange,
        GraphChange,
        ScaleChange,
        HarmonyFrameChange,
    };

    struct ScoreNoteEvent
    {
        ScoreTime duration;
        NoteValue value;
        NoteVelocity velocity;
        const MotifInstance* motifInstance;
        const Scale* scale;
        const Node* node;
    };

    using ScoreEventData = Variant<ScoreNoteEvent, Tempo, const Graph*, const Scale*, HarmonyFrame*>;

    struct ScoreEvent
    {
        ScoreTime time;
        bool committed;
        ScoreEventType type;
        ScoreEventData data;

        inline bool IsPlayNote() const
        {
            return type == ScoreEventType::PlayNote;
        }

        inline bool IsHarmonyFrame() const
        {
            return type == ScoreEventType::HarmonyFrameChange;
        }

        inline const ScoreNoteEvent& GetNoteData() const
        {
            return data.Get<ScoreNoteEvent>();
        }

        inline const Tempo GetTempoData() const
        {
            return data.Get<Tempo>();
        }

        inline const Graph* GetGraphData() const
        {
            return data.Get<const Graph*>();
        }

        inline const Scale* GetScaleData() const
        {
            return data.Get<const Scale*>();
        }

        inline HarmonyFrame* GetHarmonyFrameData()
        {
            return data.Get<HarmonyFrame*>();
        }
        
        inline const HarmonyFrame* GetHarmonyFrameData() const
        {
            return data.Get<HarmonyFrame*>();
        }
    };
}
