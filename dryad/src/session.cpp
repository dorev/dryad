#include "session.h"
#include "errors.h"
#include "scale.h"
#include <algorithm>

namespace Dryad
{

    Session::Session() = default;

    Score& Session::getScore()
    {
        return m_score;
    }

    const Score& Session::getScore() const
    {
        return m_score;
    }

    void Session::reset()
    {
        unloadAllProgressions();
        unloadAllMotifs();
        unloadAllVoices();
        m_score.resetTimeline();
        m_score.setCurrentProgression(nullptr);
        m_score.setCurrentScale(nullptr);
        m_score.setCurrentRoot(C);
    }

    Progression* Session::createProgression()
    {
        Progression* progression = m_score.create<Progression>();
        if (!progression)
            return nullptr;

        m_progressions.push_back(progression);
        if (!m_score.getCurrentProgression())
            m_score.setCurrentProgression(progression);

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
        m_score.setCurrentProgression(progression);
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

        if (m_score.getCurrentProgression() == progression)
            m_score.setCurrentProgression(nullptr);

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
        m_score.setCurrentProgression(nullptr);
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

    bool Session::insertAfter(Progression& progression, ProgressionNode* previous, ProgressionNode* node)
    {
        if (!node)
            return false;

        if (previous)
        {
            node->next = previous->next;
            previous->next = node;
        }
        else
        {
            node->next = progression.entryNode;
            progression.entryNode = node;
        }

        auto it = std::find(progression.nodes.begin(), progression.nodes.end(), node);
        if (it == progression.nodes.end())
            progression.nodes.push_back(node);

        return true;
    }

    bool Session::removeNode(Progression& progression, ProgressionNode* node)
    {
        if (!node)
            return false;

        auto nodeIt = std::find(progression.nodes.begin(), progression.nodes.end(), node);
        if (nodeIt == progression.nodes.end())
            return false;

        for (ProgressionNode* candidate : progression.nodes)
        {
            if (candidate && candidate->next == node)
                candidate->next = node->next;
        }

        for (ProgressionNode* candidate : progression.nodes)
        {
            if (!candidate)
                continue;

            ProgressionSwitchSequence* sequence = candidate->get<ProgressionSwitchSequence>();
            if (!sequence)
                continue;

            sequence->outputs.erase(
                std::remove(sequence->outputs.begin(), sequence->outputs.end(), node),
                sequence->outputs.end());
        }

        if (progression.entryNode == node)
            progression.entryNode = node->next;

        if (progression.currentProgressionChord == node->get<ProgressionChord>())
            progression.currentProgressionChord = nullptr;

        if (ProgressionChord* chord = node->get<ProgressionChord>())
            m_score.destroy(chord);
        else if (ProgressionEvent* eventNode = node->get<ProgressionEvent>())
            m_score.destroy(eventNode);
        else if (ProgressionSwitchSequence* sequence = node->get<ProgressionSwitchSequence>())
            m_score.destroy(sequence);

        progression.nodes.erase(nodeIt);
        return true;
    }

    void Session::setScale(Scale* scale)
    {
        m_score.setCurrentScale(scale);
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

        Map<const Progression*, int> progressionIndex;
        for (size_t i = 0; i < m_progressions.size(); ++i)
            progressionIndex[m_progressions[i]] = static_cast<int>(i);

        Map<const Motif*, int> motifIndex;
        for (size_t i = 0; i < m_motifs.size(); ++i)
            motifIndex[m_motifs[i]] = static_cast<int>(i);

        Map<const Voice*, int> voiceIndex;
        for (size_t i = 0; i < m_voices.size(); ++i)
            voiceIndex[m_voices[i]] = static_cast<int>(i);

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
            serializedNode.progressionChangeIndex = -1;
            serializedNode.hasScaleChange = false;
            serializedNode.motifChanges.clear();
            serializedNode.voiceChanges.clear();

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
                serializedNode.type = SerializedProgressionNodeType::Event;
                serializedNode.scoreEnd = eventNode->scoreEnd;

                if (eventNode->progressionChange)
                {
                    auto progressionIt = progressionIndex.find(eventNode->progressionChange);
                    if (progressionIt == progressionIndex.end())
                        return Invalid;

                    serializedNode.progressionChangeIndex = progressionIt->second;
                }

                if (eventNode->scaleChange)
                {
                    serializedNode.hasScaleChange = true;
                    for (int degreeIndex = 0; degreeIndex < 7; ++degreeIndex)
                    {
                        serializedNode.scaleOffsets[degreeIndex] = eventNode->scaleChange->noteOffsets.degrees[degreeIndex];
                        serializedNode.scaleDegreeQualities[degreeIndex] = eventNode->scaleChange->degreeQualities.degrees[degreeIndex];
                    }
                }

                serializedNode.motifChanges.reserve(eventNode->motifChanges.size());
                for (const MotifChange& change : eventNode->motifChanges)
                {
                    auto oldIt = motifIndex.find(change.oldMotif);
                    auto newIt = motifIndex.find(change.newMotif);
                    if (oldIt == motifIndex.end() || newIt == motifIndex.end())
                        return Invalid;

                    SerializedMotifChange serializedChange;
                    serializedChange.oldMotifIndex = oldIt->second;
                    serializedChange.newMotifIndex = newIt->second;
                    serializedNode.motifChanges.push_back(serializedChange);
                }

                serializedNode.voiceChanges.reserve(eventNode->voiceChanges.size());
                for (const VoiceChange& change : eventNode->voiceChanges)
                {
                    auto oldIt = voiceIndex.find(change.oldVoice);
                    auto newIt = voiceIndex.find(change.newVoice);
                    if (oldIt == voiceIndex.end() || newIt == voiceIndex.end())
                        return Invalid;

                    SerializedVoiceChange serializedChange;
                    serializedChange.oldVoiceIndex = oldIt->second;
                    serializedChange.newVoiceIndex = newIt->second;
                    serializedNode.voiceChanges.push_back(serializedChange);
                }
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

        Vector<SerializedMotifNote> notes;
        motif.forEachEdge<MotifNote>([&](const MotifNote* note)
            {
                SerializedMotifNote serializedNote;
                serializedNote.relativeValue = note->relativeValue;
                serializedNote.duration = note->duration;
                serializedNote.offset = note->offset;
                notes.push_back(serializedNote);
            });

        std::sort(notes.begin(), notes.end(), [](const SerializedMotifNote& a, const SerializedMotifNote& b)
            {
                if (a.offset != b.offset)
                    return a.offset < b.offset;
                if (a.duration != b.duration)
                    return a.duration < b.duration;
                return a.relativeValue < b.relativeValue;
            });

        outSerialized.notes = std::move(notes);

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
        outSerialized.voices.clear();
        outSerialized.currentRoot = m_score.getCurrentRoot();
        outSerialized.hasScale = (m_score.getCurrentScale() != nullptr);

        if (outSerialized.hasScale)
        {
            for (int degreeIndex = 0; degreeIndex < 7; ++degreeIndex)
            {
                outSerialized.scaleOffsets[degreeIndex] = m_score.getCurrentScale()->noteOffsets.degrees[degreeIndex];
                outSerialized.scaleDegreeQualities[degreeIndex] = m_score.getCurrentScale()->degreeQualities.degrees[degreeIndex];
            }
        }

        Vector<Progression*> progressionsToSerialize = m_progressions;
        if (progressionsToSerialize.empty() && m_score.getCurrentProgression())
            progressionsToSerialize.push_back(m_score.getCurrentProgression());

        outSerialized.progressions.reserve(progressionsToSerialize.size());
        for (Progression* progression : progressionsToSerialize)
        {
            if (!progression)
                return Invalid;

            SerializedProgression serializedProgression;
            Error error = serializeProgression(*progression, serializedProgression);
            if (error)
                return error;

            if (progression == m_score.getCurrentProgression())
                outSerialized.activeProgressionIndex = static_cast<int>(outSerialized.progressions.size());

            outSerialized.progressions.push_back(std::move(serializedProgression));
        }

        Vector<const Motif*> motifsToSerialize;
        motifsToSerialize.reserve(m_motifs.size());
        for (Motif* motif : m_motifs)
            motifsToSerialize.push_back(motif);

        if (motifsToSerialize.empty())
        {
            m_score.forEachEdge<Motif>([&](const Motif* motif)
                {
                    motifsToSerialize.push_back(motif);
                });
        }

        outSerialized.motifs.reserve(motifsToSerialize.size());
        for (const Motif* motif : motifsToSerialize)
        {
            if (!motif)
                return Invalid;

            SerializedMotif serializedMotif;
            Error error = serializeMotif(*motif, serializedMotif);
            if (error)
                return error;

            outSerialized.motifs.push_back(std::move(serializedMotif));
        }

        Vector<const Voice*> voicesToSerialize;
        voicesToSerialize.reserve(m_voices.size());
        for (Voice* voice : m_voices)
            voicesToSerialize.push_back(voice);
        if (voicesToSerialize.empty())
        {
            m_score.forEachEdge<Voice>([&](const Voice* voice)
                {
                    voicesToSerialize.push_back(voice);
                });
        }

        Map<const Motif*, int> motifIndex;
        for (size_t i = 0; i < motifsToSerialize.size(); ++i)
            motifIndex[motifsToSerialize[i]] = static_cast<int>(i);

        outSerialized.voices.reserve(voicesToSerialize.size());
        for (const Voice* voice : voicesToSerialize)
        {
            if (!voice)
                return Invalid;

            SerializedVoiceDefinition serializedVoice;
            serializedVoice.id = voice->id;
            serializedVoice.name = voice->name;
            serializedVoice.motifIndices.clear();

            serializedVoice.motifIndices.reserve(voice->motifs.size());
            for (Motif* motif : voice->motifs)
            {
                auto motifIt = motifIndex.find(motif);
                if (motifIt == motifIndex.end())
                    return Invalid;

                serializedVoice.motifIndices.push_back(motifIt->second);
            }

            outSerialized.voices.push_back(std::move(serializedVoice));
        }

        return Success;
    }

    Error Session::deserializeGraph(const SerializedGraph& serialized)
    {
        unloadAllProgressions();
        unloadAllMotifs();
        unloadAllVoices();

        m_score.setCurrentRoot(serialized.currentRoot);
        if (serialized.hasScale)
        {
            ScaleNoteOffsets offsets(
                serialized.scaleOffsets[0],
                serialized.scaleOffsets[1],
                serialized.scaleOffsets[2],
                serialized.scaleOffsets[3],
                serialized.scaleOffsets[4],
                serialized.scaleOffsets[5],
                serialized.scaleOffsets[6]
            );

            ScaleDegreeQualities qualities(
                serialized.scaleDegreeQualities[0],
                serialized.scaleDegreeQualities[1],
                serialized.scaleDegreeQualities[2],
                serialized.scaleDegreeQualities[3],
                serialized.scaleDegreeQualities[4],
                serialized.scaleDegreeQualities[5],
                serialized.scaleDegreeQualities[6]
            );

            m_score.setCurrentScale(m_score.create<Scale>(offsets, qualities));
        }
        else
        {
            m_score.setCurrentScale(nullptr);
        }

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
            m_score.setCurrentProgression(m_progressions[serialized.activeProgressionIndex]);
        }

        for (const SerializedMotif& motif : serialized.motifs)
        {
            Motif* createdMotif = nullptr;
            Error error = deserializeMotif(motif, createdMotif);
            if (error)
                return error;

            m_motifs.push_back(createdMotif);
        }

        m_voices.reserve(serialized.voices.size());
        for (const SerializedVoiceDefinition& voice : serialized.voices)
        {
            Voice* createdVoice = m_score.addVoice(voice.id, voice.name);
            if (!createdVoice)
                return Invalid;

            for (int motifIndex : voice.motifIndices)
            {
                if (motifIndex < 0 || motifIndex >= static_cast<int>(m_motifs.size()))
                    return Invalid;

                Error error = createdVoice->addMotif(m_motifs[motifIndex]);
                if (error)
                    return error;
            }

            m_voices.push_back(createdVoice);
        }

        for (size_t progressionIndex = 0; progressionIndex < serialized.progressions.size(); ++progressionIndex)
        {
            const SerializedProgression& serializedProgression = serialized.progressions[progressionIndex];
            Progression* progression = m_progressions[progressionIndex];
            if (!progression)
                return Invalid;

            if (serializedProgression.nodes.size() != progression->nodes.size())
                return Invalid;

            for (size_t nodeIndex = 0; nodeIndex < serializedProgression.nodes.size(); ++nodeIndex)
            {
                const SerializedProgressionNode& serializedNode = serializedProgression.nodes[nodeIndex];
                if (serializedNode.type != SerializedProgressionNodeType::Event)
                    continue;

                ProgressionEvent* eventNode = progression->nodes[nodeIndex]->get<ProgressionEvent>();
                if (!eventNode)
                    return Invalid;

                eventNode->scoreEnd = serializedNode.scoreEnd;

                if (serializedNode.progressionChangeIndex >= 0)
                {
                    if (serializedNode.progressionChangeIndex >= static_cast<int>(m_progressions.size()))
                        return Invalid;

                    eventNode->progressionChange = m_progressions[serializedNode.progressionChangeIndex];
                }

                if (serializedNode.hasScaleChange)
                {
                    ScaleNoteOffsets offsets(
                        serializedNode.scaleOffsets[0],
                        serializedNode.scaleOffsets[1],
                        serializedNode.scaleOffsets[2],
                        serializedNode.scaleOffsets[3],
                        serializedNode.scaleOffsets[4],
                        serializedNode.scaleOffsets[5],
                        serializedNode.scaleOffsets[6]
                    );

                    ScaleDegreeQualities qualities(
                        serializedNode.scaleDegreeQualities[0],
                        serializedNode.scaleDegreeQualities[1],
                        serializedNode.scaleDegreeQualities[2],
                        serializedNode.scaleDegreeQualities[3],
                        serializedNode.scaleDegreeQualities[4],
                        serializedNode.scaleDegreeQualities[5],
                        serializedNode.scaleDegreeQualities[6]
                    );

                    eventNode->scaleChange = m_score.create<Scale>(offsets, qualities);
                }

                eventNode->motifChanges.clear();
                eventNode->motifChanges.reserve(serializedNode.motifChanges.size());
                for (const SerializedMotifChange& change : serializedNode.motifChanges)
                {
                    if (change.oldMotifIndex < 0 || change.oldMotifIndex >= static_cast<int>(m_motifs.size()))
                        return Invalid;
                    if (change.newMotifIndex < 0 || change.newMotifIndex >= static_cast<int>(m_motifs.size()))
                        return Invalid;

                    MotifChange motifChange;
                    motifChange.oldMotif = m_motifs[change.oldMotifIndex];
                    motifChange.newMotif = m_motifs[change.newMotifIndex];
                    eventNode->motifChanges.push_back(motifChange);
                }

                eventNode->voiceChanges.clear();
                eventNode->voiceChanges.reserve(serializedNode.voiceChanges.size());
                for (const SerializedVoiceChange& change : serializedNode.voiceChanges)
                {
                    if (change.oldVoiceIndex < 0 || change.oldVoiceIndex >= static_cast<int>(m_voices.size()))
                        return Invalid;
                    if (change.newVoiceIndex < 0 || change.newVoiceIndex >= static_cast<int>(m_voices.size()))
                        return Invalid;

                    VoiceChange voiceChange;
                    voiceChange.oldVoice = m_voices[change.oldVoiceIndex];
                    voiceChange.newVoice = m_voices[change.newVoiceIndex];
                    eventNode->voiceChanges.push_back(voiceChange);
                }
            }
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
