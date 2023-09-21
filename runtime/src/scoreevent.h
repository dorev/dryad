#pragma once

#include "types.h"
#include "motifinstance.h"

namespace Dryad
{
    class Graph;
    class Scale;
    class HarmonyFrame;

    enum class ScoreEventType
    {
        Undefined,
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

        const Motif* GetMotif() const
        {
            if (motifInstance != nullptr)
            {
                return motifInstance->GetMotif();
            }
            return nullptr;
        }
    };

    using ScoreEventData = Variant<ScoreNoteEvent, Tempo, const Graph*, const Scale*, HarmonyFrame*>;

    class ScoreEvent
    {
    public:
        ScoreTime time;
        bool committed;
        ScoreEventType type;
        ScoreEventData data;

        ScoreEvent()
            : time(0)
            , committed(false)
            , type(ScoreEventType::Undefined)
            , data(static_cast<Graph*>(nullptr))
        {
        }

        inline void SetNote(const ScoreNoteEvent& noteEvent)
        {
            type = ScoreEventType::PlayNote;
            data = noteEvent;
        }

        inline void SetTempo(const Tempo& tempo)
        {
            type = ScoreEventType::TempoChange;
            data = tempo;
        }

        inline void SetGraph(const Graph* graph)
        {
            type = ScoreEventType::GraphChange;
            data = graph;
        }

        inline void SetScale(const Scale* scale)
        {
            type = ScoreEventType::ScaleChange;
            data = scale;
        }

        inline void SetHarmonyFrame(HarmonyFrame* harmonyFrame)
        {
            type = ScoreEventType::HarmonyFrameChange;
            data = harmonyFrame;
        }

        inline bool IsPlayNote() const
        {
            return type == ScoreEventType::PlayNote;
        }

        inline bool IsHarmonyFrame() const
        {
            return type == ScoreEventType::HarmonyFrameChange;
        }

        inline bool IsTempoChange() const
        {
            return type == ScoreEventType::TempoChange;
        }

        inline bool IsGraphChange() const
        {
            return type == ScoreEventType::GraphChange;
        }

        inline bool IsScaleChange() const
        {
            return type == ScoreEventType::ScaleChange;
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
