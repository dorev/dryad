#pragma once

#include "events.h"
#include "progression.h"
#include "score.h"
#include "serialize.h"
#include "graph_io.h"

namespace Dryad
{

    class Session
    {
    public:
        Session();

        Score& getScore();
        const Score& getScore() const;

        Progression* createProgression();
        void setActiveProgression(Progression* progression);
        const Vector<Progression*>& getProgressions() const;
        bool unloadProgression(Progression* progression);
        void unloadAllProgressions();

        Motif* createMotif();
        const Vector<Motif*>& getMotifs() const;
        bool unloadMotif(Motif* motif);
        void unloadAllMotifs();

        Voice* createVoice(int id, const String& name);
        const Vector<Voice*>& getVoices() const;
        bool unloadVoice(Voice* voice);
        void unloadAllVoices();
        ProgressionChord* addChord(Progression& progression, const Chord& chord, Time duration);
        ProgressionEvent* addEvent(Progression& progression);
        ProgressionSwitchSequence* addSwitch(Progression& progression);
        void setEntry(Progression& progression, ProgressionNode* entryNode);
        void link(ProgressionNode* from, ProgressionNode* to);
        bool insertAfter(Progression& progression, ProgressionNode* previous, ProgressionNode* node);
        bool removeNode(Progression& progression, ProgressionNode* node);

        void setScale(Scale* scale);
        void setProgression(Progression* progression);

        Error tick(Time delta, Vector<ScoreEvent>& outEvents);

        Error serializeProgression(const Progression& progression, SerializedProgression& outSerialized) const;
        Error deserializeProgression(const SerializedProgression& serialized, Progression*& outProgression);

        Error serializeMotif(const Motif& motif, SerializedMotif& outSerialized) const;
        Error deserializeMotif(const SerializedMotif& serialized, Motif*& outMotif);

        Error serializeGraph(SerializedGraph& outSerialized) const;
        Error deserializeGraph(const SerializedGraph& serialized);
        Error saveGraphToFile(const String& path) const;
        Error loadGraphFromFile(const String& path);
        void reset();

    private:
        Score m_score;
        Vector<Progression*> m_progressions;
        Vector<Motif*> m_motifs;
        Vector<Voice*> m_voices;
    };

} // namespace Dryad
