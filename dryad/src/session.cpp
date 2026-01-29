#include "session.h"
#include "errors.h"
#include <algorithm>

namespace Dryad
{

    Session::Session() = default;

    Score& Session::getScore()
    {
        return m_score;
    }

    Progression* Session::createProgression()
    {
        Progression* progression = m_score.create<Progression>();
        if (!progression)
            return nullptr;

        m_progressions.push_back(progression);
        if (!m_score.currentProgression)
            m_score.currentProgression = progression;

        return progression;
    }

    Motif* Session::createMotif()
    {
        Motif* motif = m_score.create<Motif>();
        if (!motif)
            return nullptr;

        m_motifs.push_back(motif);
        return motif;
    }

    const Vector<Motif*>& Session::getMotifs() const
    {
        return m_motifs;
    }

    bool Session::unloadMotif(Motif* motif)
    {
        if (!motif)
            return false;

        auto it = std::find(m_motifs.begin(), m_motifs.end(), motif);
        if (it == m_motifs.end())
            return false;

        m_motifs.erase(it);
        m_score.destroy(motif);
        return true;
    }

    void Session::unloadAllMotifs()
    {
        for (Motif* motif : m_motifs)
            m_score.destroy(motif);

        m_motifs.clear();
    }

    Voice* Session::createVoice(int id, const String& name)
    {
        Voice* voice = m_score.addVoice(id, name);
        if (!voice)
            return nullptr;

        if (std::find(m_voices.begin(), m_voices.end(), voice) == m_voices.end())
            m_voices.push_back(voice);

        return voice;
    }

    const Vector<Voice*>& Session::getVoices() const
    {
        return m_voices;
    }

    bool Session::unloadVoice(Voice* voice)
    {
        if (!voice)
            return false;

        auto it = std::find(m_voices.begin(), m_voices.end(), voice);
        if (it == m_voices.end())
            return false;

        m_voices.erase(it);
        m_score.destroy(voice);
        return true;
    }

    void Session::unloadAllVoices()
    {
        for (Voice* voice : m_voices)
            m_score.destroy(voice);

        m_voices.clear();
    }

    void Session::setActiveProgression(Progression* progression)
    {
        m_score.currentProgression = progression;
        if (progression && std::find(m_progressions.begin(), m_progressions.end(), progression) == m_progressions.end())
            m_progressions.push_back(progression);
    }

    const Vector<Progression*>& Session::getProgressions() const
    {
        return m_progressions;
    }

    bool Session::unloadProgression(Progression* progression)
    {
        if (!progression)
            return false;

        auto it = std::find(m_progressions.begin(), m_progressions.end(), progression);
        if (it == m_progressions.end())
            return false;

        if (m_score.currentProgression == progression)
            m_score.currentProgression = nullptr;

        auto destroyNode = [&](ProgressionNode* node)
        {
            if (!node)
                return;

            if (ProgressionChord* chord = node->get<ProgressionChord>())
                m_score.destroy(chord);
            else if (ProgressionEvent* eventNode = node->get<ProgressionEvent>())
                m_score.destroy(eventNode);
            else if (ProgressionSwitchSequence* sequence = node->get<ProgressionSwitchSequence>())
                m_score.destroy(sequence);
        };

        for (ProgressionNode* node : progression->nodes)
            destroyNode(node);

        progression->nodes.clear();
        m_score.destroy(progression);
        m_progressions.erase(it);
        return true;
    }

    void Session::unloadAllProgressions()
    {
        for (Progression* progression : m_progressions)
        {
            if (!progression)
                continue;

            auto destroyNode = [&](ProgressionNode* node)
            {
                if (!node)
                    return;

                if (ProgressionChord* chord = node->get<ProgressionChord>())
                    m_score.destroy(chord);
                else if (ProgressionEvent* eventNode = node->get<ProgressionEvent>())
                    m_score.destroy(eventNode);
                else if (ProgressionSwitchSequence* sequence = node->get<ProgressionSwitchSequence>())
                    m_score.destroy(sequence);
            };

            for (ProgressionNode* node : progression->nodes)
                destroyNode(node);

            progression->nodes.clear();
            m_score.destroy(progression);
        }

        m_progressions.clear();
        m_score.currentProgression = nullptr;
    }

    ProgressionChord* Session::addChord(Progression& progression, const Chord& chord, Time duration)
    {
        ProgressionChord* node = m_score.create<ProgressionChord>(chord, duration);
        if (!node)
            return nullptr;

        progression.nodes.push_back(node);
        return node;
    }

    ProgressionEvent* Session::addEvent(Progression& progression)
    {
        ProgressionEvent* node = m_score.create<ProgressionEvent>();
        if (!node)
            return nullptr;

        progression.nodes.push_back(node);
        return node;
    }

    ProgressionSwitchSequence* Session::addSwitch(Progression& progression)
    {
        ProgressionSwitchSequence* node = m_score.create<ProgressionSwitchSequence>();
        if (!node)
            return nullptr;

        progression.nodes.push_back(node);
        return node;
    }

    void Session::setEntry(Progression& progression, ProgressionNode* entryNode)
    {
        progression.entryNode = entryNode;
    }

    void Session::link(ProgressionNode* from, ProgressionNode* to)
    {
        if (from)
            from->next = to;
    }

    void Session::setScale(Scale* scale)
    {
        m_score.currentScale = scale;
    }

    void Session::setProgression(Progression* progression)
    {
        setActiveProgression(progression);
    }

    Error Session::tick(Time delta, Vector<ScoreEvent>& outEvents)
    {
        return m_score.tick(delta, outEvents);
    }

    Error Session::serializeProgression(const Progression& progression, SerializedProgression& outSerialized) const
    {
        outSerialized.nodes.clear();
        outSerialized.entryIndex = -1;

        Map<const ProgressionNode*, int> indexByNode;
        indexByNode.clear();

        int index = 0;
        for (ProgressionNode* node : progression.nodes)
            indexByNode[node] = index++;

        if (progression.entryNode)
        {
            auto entryIt = indexByNode.find(progression.entryNode);
            if (entryIt == indexByNode.end())
                return Invalid;

            outSerialized.entryIndex = entryIt->second;
        }

        outSerialized.nodes.reserve(progression.nodes.size());

        for (ProgressionNode* node : progression.nodes)
        {
            SerializedProgressionNode serializedNode;
            serializedNode.nextIndex = -1;
            serializedNode.outputs.clear();
            serializedNode.scoreEnd = false;

            if (!node)
                return Invalid;

            auto nextIt = node->next ? indexByNode.find(node->next) : indexByNode.end();
            if (node->next && nextIt == indexByNode.end())
                return Invalid;

            if (nextIt != indexByNode.end())
                serializedNode.nextIndex = nextIt->second;

            if (ProgressionChord* chord = node->get<ProgressionChord>())
            {
                serializedNode.type = SerializedProgressionNodeType::Chord;
                serializedNode.chord = chord->chord;
                serializedNode.duration = chord->duration;
            }
            else if (ProgressionSwitchSequence* sequence = node->get<ProgressionSwitchSequence>())
            {
                serializedNode.type = SerializedProgressionNodeType::SwitchSequence;
                serializedNode.outputs.reserve(sequence->outputs.size());
                for (ProgressionNode* output : sequence->outputs)
                {
                    auto outputIt = indexByNode.find(output);
                    if (outputIt == indexByNode.end())
                        return Invalid;

                    serializedNode.outputs.push_back(outputIt->second);
                }
            }
            else if (ProgressionEvent* eventNode = node->get<ProgressionEvent>())
            {
                if (eventNode->scaleChange || eventNode->progressionChange || !eventNode->motifChanges.empty() || !eventNode->voiceChanges.empty())
                    return NotImplemented;

                serializedNode.type = SerializedProgressionNodeType::Event;
                serializedNode.scoreEnd = eventNode->scoreEnd;
            }
            else
            {
                return NotImplemented;
            }

            outSerialized.nodes.push_back(std::move(serializedNode));
        }

        return Success;
    }

    Error Session::deserializeProgression(const SerializedProgression& serialized, Progression*& outProgression)
    {
        if (serialized.nodes.empty())
            return Invalid;

        Progression* progression = m_score.create<Progression>();
        if (!progression)
            return Invalid;

        Vector<ProgressionNode*> nodes;
        nodes.reserve(serialized.nodes.size());

        for (const SerializedProgressionNode& serializedNode : serialized.nodes)
        {
            ProgressionNode* node = nullptr;
            switch (serializedNode.type)
            {
            case SerializedProgressionNodeType::Chord:
                node = m_score.create<ProgressionChord>(serializedNode.chord, serializedNode.duration);
                break;
            case SerializedProgressionNodeType::Event:
            {
                ProgressionEvent* eventNode = m_score.create<ProgressionEvent>();
                if (eventNode)
                    eventNode->scoreEnd = serializedNode.scoreEnd;
                node = eventNode;
                break;
            }
            case SerializedProgressionNodeType::SwitchSequence:
                node = m_score.create<ProgressionSwitchSequence>();
                break;
            default:
                return Invalid;
            }

            if (!node)
                return Invalid;

            nodes.push_back(node);
            progression->nodes.push_back(node);
        }

        for (size_t i = 0; i < serialized.nodes.size(); ++i)
        {
            const SerializedProgressionNode& serializedNode = serialized.nodes[i];
            ProgressionNode* node = nodes[i];

            if (serializedNode.nextIndex >= 0)
            {
                if (serializedNode.nextIndex >= static_cast<int>(nodes.size()))
                    return Invalid;

                node->next = nodes[serializedNode.nextIndex];
            }

            if (serializedNode.type == SerializedProgressionNodeType::SwitchSequence)
            {
                ProgressionSwitchSequence* sequence = node->get<ProgressionSwitchSequence>();
                if (!sequence)
                    return Invalid;

                sequence->outputs.clear();
                for (int outputIndex : serializedNode.outputs)
                {
                    if (outputIndex < 0 || outputIndex >= static_cast<int>(nodes.size()))
                        return Invalid;

                    sequence->outputs.push_back(nodes[outputIndex]);
                }
            }
        }

        if (serialized.entryIndex >= 0)
        {
            if (serialized.entryIndex >= static_cast<int>(nodes.size()))
                return Invalid;

            progression->entryNode = nodes[serialized.entryIndex];
        }

        outProgression = progression;
        return Success;
    }

    Error Session::serializeMotif(const Motif& motif, SerializedMotif& outSerialized) const
    {
        outSerialized.notes.clear();
        outSerialized.harmonicAnchor = motif.harmonicAnchor;
        outSerialized.rhythmicAnchor = motif.rhythmicAnchor;
        outSerialized.noteIntervalType = motif.noteIntervalType;
        outSerialized.duration = motif.duration;

        Motif& mutableMotif = const_cast<Motif&>(motif);
        mutableMotif.forEachEdge<MotifNote>([&](MotifNote* note)
            {
                SerializedMotifNote serializedNote;
                serializedNote.relativeValue = note->relativeValue;
                serializedNote.duration = note->duration;
                serializedNote.offset = note->offset;
                outSerialized.notes.push_back(serializedNote);
            });

        return Success;
    }

    Error Session::deserializeMotif(const SerializedMotif& serialized, Motif*& outMotif)
    {
        Motif* motif = m_score.create<Motif>();
        if (!motif)
            return Invalid;

        motif->harmonicAnchor = serialized.harmonicAnchor;
        motif->rhythmicAnchor = serialized.rhythmicAnchor;
        motif->noteIntervalType = serialized.noteIntervalType;

        for (const SerializedMotifNote& note : serialized.notes)
        {
            if (!motif->addNote(note.relativeValue, note.duration, note.offset))
                return Invalid;
        }

        motif->duration = serialized.duration;
        outMotif = motif;
        return Success;
    }

    Error Session::serializeGraph(SerializedGraph& outSerialized) const
    {
        outSerialized.motifs.clear();
        outSerialized.progressions.clear();
        outSerialized.activeProgressionIndex = -1;

        Vector<Progression*> progressionsToSerialize = m_progressions;
        if (progressionsToSerialize.empty() && m_score.currentProgression)
            progressionsToSerialize.push_back(m_score.currentProgression);

        outSerialized.progressions.reserve(progressionsToSerialize.size());
        for (Progression* progression : progressionsToSerialize)
        {
            if (!progression)
                return Invalid;

            SerializedProgression serializedProgression;
            Error error = serializeProgression(*progression, serializedProgression);
            if (error)
                return error;

            if (progression == m_score.currentProgression)
                outSerialized.activeProgressionIndex = static_cast<int>(outSerialized.progressions.size());

            outSerialized.progressions.push_back(std::move(serializedProgression));
        }

        Vector<Motif*> motifsToSerialize = m_motifs;
        if (motifsToSerialize.empty())
        {
            Score& mutableScore = const_cast<Score&>(m_score);
            mutableScore.forEachEdge<Motif>([&](Motif* motif)
                {
                    motifsToSerialize.push_back(motif);
                });
        }

        outSerialized.motifs.reserve(motifsToSerialize.size());
        for (Motif* motif : motifsToSerialize)
        {
            if (!motif)
                return Invalid;

            SerializedMotif serializedMotif;
            Error error = serializeMotif(*motif, serializedMotif);
            if (error)
                return error;

            outSerialized.motifs.push_back(std::move(serializedMotif));
        }

        return Success;
    }

    Error Session::deserializeGraph(const SerializedGraph& serialized)
    {
        unloadAllProgressions();
        unloadAllMotifs();
        unloadAllVoices();

        for (const SerializedProgression& progression : serialized.progressions)
        {
            Progression* createdProgression = nullptr;
            Error error = deserializeProgression(progression, createdProgression);
            if (error)
                return error;

            m_progressions.push_back(createdProgression);
        }

        if (serialized.activeProgressionIndex >= 0 &&
            serialized.activeProgressionIndex < static_cast<int>(m_progressions.size()))
        {
            m_score.currentProgression = m_progressions[serialized.activeProgressionIndex];
        }

        for (const SerializedMotif& motif : serialized.motifs)
        {
            Motif* createdMotif = nullptr;
            Error error = deserializeMotif(motif, createdMotif);
            if (error)
                return error;

            m_motifs.push_back(createdMotif);
        }

        return Success;
    }

    Error Session::saveGraphToFile(const String& path) const
    {
        SerializedGraph graph;
        Error error = serializeGraph(graph);
        if (error)
            return error;

        return writeGraphToFile(graph, path);
    }

    Error Session::loadGraphFromFile(const String& path)
    {
        SerializedGraph graph;
        Error error = readGraphFromFile(path, graph);
        if (error)
            return error;

        return deserializeGraph(graph);
    }

} // namespace Dryad
