#include "graph_io.h"
#include <cstdint>
#include <fstream>

namespace Dryad
{

    namespace
    {
        constexpr char kMagic[] = "DRYADGPH";
        constexpr uint32_t kVersion = 0;

        bool writeBytes(std::ofstream& stream, const void* data, size_t size)
        {
            stream.write(static_cast<const char*>(data), static_cast<std::streamsize>(size));
            return stream.good();
        }

        bool readBytes(std::ifstream& stream, void* data, size_t size)
        {
            stream.read(static_cast<char*>(data), static_cast<std::streamsize>(size));
            return stream.good();
        }

        bool writeUint32(std::ofstream& stream, uint32_t value)
        {
            return writeBytes(stream, &value, sizeof(value));
        }

        bool writeInt32(std::ofstream& stream, int32_t value)
        {
            return writeBytes(stream, &value, sizeof(value));
        }

        bool writeUint8(std::ofstream& stream, uint8_t value)
        {
            return writeBytes(stream, &value, sizeof(value));
        }

        bool readUint32(std::ifstream& stream, uint32_t& value)
        {
            return readBytes(stream, &value, sizeof(value));
        }

        bool readInt32(std::ifstream& stream, int32_t& value)
        {
            return readBytes(stream, &value, sizeof(value));
        }

        bool readUint8(std::ifstream& stream, uint8_t& value)
        {
            return readBytes(stream, &value, sizeof(value));
        }

        bool writeString(std::ofstream& stream, const String& value)
        {
            if (!writeUint32(stream, static_cast<uint32_t>(value.size())))
                return false;
            return writeBytes(stream, value.data(), value.size());
        }

        bool readString(std::ifstream& stream, String& value)
        {
            uint32_t length = 0;
            if (!readUint32(stream, length))
                return false;

            value.resize(length);
            if (length == 0)
                return true;

            return readBytes(stream, value.data(), length);
        }
    }

    Error writeGraphToFile(const SerializedGraph& graph, const String& path)
    {
        std::ofstream stream(path, std::ios::binary);
        if (!stream.is_open())
            return Invalid;

        if (!writeBytes(stream, kMagic, sizeof(kMagic) - 1))
            return Invalid;
        if (!writeUint32(stream, kVersion))
            return Invalid;

        if (!writeUint32(stream, static_cast<uint32_t>(graph.progressions.size())))
            return Invalid;
        if (!writeInt32(stream, static_cast<int32_t>(graph.activeProgressionIndex)))
            return Invalid;

        if (!writeUint32(stream, static_cast<uint32_t>(graph.currentRoot)))
            return Invalid;
        if (!writeUint8(stream, static_cast<uint8_t>(graph.hasScale ? 1 : 0)))
            return Invalid;

        if (graph.hasScale)
        {
            for (int i = 0; i < 7; ++i)
            {
                if (!writeInt32(stream, static_cast<int32_t>(graph.scaleOffsets[i])))
                    return Invalid;
                if (!writeInt32(stream, static_cast<int32_t>(graph.scaleDegreeQualities[i])))
                    return Invalid;
            }
        }

        for (const SerializedProgression& progression : graph.progressions)
        {
            if (!writeUint32(stream, static_cast<uint32_t>(progression.nodes.size())))
                return Invalid;
            if (!writeInt32(stream, static_cast<int32_t>(progression.entryIndex)))
                return Invalid;

            for (const SerializedProgressionNode& node : progression.nodes)
            {
                if (!writeUint32(stream, static_cast<uint32_t>(node.type)))
                    return Invalid;
                if (!writeInt32(stream, static_cast<int32_t>(node.nextIndex)))
                    return Invalid;
                if (!writeUint32(stream, static_cast<uint32_t>(node.outputs.size())))
                    return Invalid;

                for (int outputIndex : node.outputs)
                {
                    if (!writeInt32(stream, static_cast<int32_t>(outputIndex)))
                        return Invalid;
                }

                if (!writeInt32(stream, static_cast<int32_t>(node.chord.degree)))
                    return Invalid;
                if (!writeInt32(stream, static_cast<int32_t>(node.chord.qualities)))
                    return Invalid;
                if (!writeInt32(stream, static_cast<int32_t>(node.chord.accidental)))
                    return Invalid;
                if (!writeUint32(stream, static_cast<uint32_t>(node.duration)))
                    return Invalid;
                if (!writeUint8(stream, static_cast<uint8_t>(node.scoreEnd ? 1 : 0)))
                    return Invalid;

                if (!writeInt32(stream, static_cast<int32_t>(node.progressionChangeIndex)))
                    return Invalid;
                if (!writeUint8(stream, static_cast<uint8_t>(node.hasScaleChange ? 1 : 0)))
                    return Invalid;
                if (node.hasScaleChange)
                {
                    for (int i = 0; i < 7; ++i)
                    {
                        if (!writeInt32(stream, static_cast<int32_t>(node.scaleOffsets[i])))
                            return Invalid;
                        if (!writeInt32(stream, static_cast<int32_t>(node.scaleDegreeQualities[i])))
                            return Invalid;
                    }
                }

                if (!writeUint32(stream, static_cast<uint32_t>(node.motifChanges.size())))
                    return Invalid;
                for (const SerializedMotifChange& change : node.motifChanges)
                {
                    if (!writeInt32(stream, static_cast<int32_t>(change.oldMotifIndex)))
                        return Invalid;
                    if (!writeInt32(stream, static_cast<int32_t>(change.newMotifIndex)))
                        return Invalid;
                }

                if (!writeUint32(stream, static_cast<uint32_t>(node.voiceChanges.size())))
                    return Invalid;
                for (const SerializedVoiceChange& change : node.voiceChanges)
                {
                    if (!writeInt32(stream, static_cast<int32_t>(change.oldVoiceIndex)))
                        return Invalid;
                    if (!writeInt32(stream, static_cast<int32_t>(change.newVoiceIndex)))
                        return Invalid;
                }
            }
        }

        if (!writeUint32(stream, static_cast<uint32_t>(graph.voices.size())))
            return Invalid;

        for (const SerializedVoiceDefinition& voice : graph.voices)
        {
            if (!writeInt32(stream, static_cast<int32_t>(voice.id)))
                return Invalid;
            if (!writeString(stream, voice.name))
                return Invalid;
            if (!writeUint32(stream, static_cast<uint32_t>(voice.motifIndices.size())))
                return Invalid;

            for (int motifIndex : voice.motifIndices)
            {
                if (!writeInt32(stream, static_cast<int32_t>(motifIndex)))
                    return Invalid;
            }
        }

        if (!writeUint32(stream, static_cast<uint32_t>(graph.motifs.size())))
            return Invalid;

        for (const SerializedMotif& motif : graph.motifs)
        {
            if (!writeInt32(stream, static_cast<int32_t>(motif.harmonicAnchor)))
                return Invalid;
            if (!writeInt32(stream, static_cast<int32_t>(motif.rhythmicAnchor)))
                return Invalid;
            if (!writeInt32(stream, static_cast<int32_t>(motif.noteIntervalType)))
                return Invalid;
            if (!writeUint32(stream, static_cast<uint32_t>(motif.duration)))
                return Invalid;

            if (!writeUint32(stream, static_cast<uint32_t>(motif.notes.size())))
                return Invalid;

            for (const SerializedMotifNote& note : motif.notes)
            {
                if (!writeInt32(stream, static_cast<int32_t>(note.relativeValue)))
                    return Invalid;
                if (!writeUint32(stream, static_cast<uint32_t>(note.duration)))
                    return Invalid;
                if (!writeUint32(stream, static_cast<uint32_t>(note.offset)))
                    return Invalid;
            }
        }

        return Success;
    }

    Error readGraphFromFile(const String& path, SerializedGraph& graph)
    {
        std::ifstream stream(path, std::ios::binary);
        if (!stream.is_open())
            return Invalid;

        char magic[sizeof(kMagic) - 1] = {};
        if (!readBytes(stream, magic, sizeof(magic)))
            return Invalid;

        if (std::string(magic, sizeof(magic)) != kMagic)
            return Invalid;

        uint32_t version = 0;
        if (!readUint32(stream, version))
            return Invalid;

        if (version != kVersion)
            return Invalid;

        graph.progressions.clear();
        graph.activeProgressionIndex = -1;
        graph.currentRoot = 0;
        graph.hasScale = false;
        graph.voices.clear();

        uint32_t progressionCount = 0;
        int32_t activeIndex = -1;

        if (!readUint32(stream, progressionCount))
            return Invalid;
        if (!readInt32(stream, activeIndex))
            return Invalid;

        graph.progressions.reserve(progressionCount);
        graph.activeProgressionIndex = activeIndex;

        uint32_t currentRoot = 0;
        if (!readUint32(stream, currentRoot))
            return Invalid;
        graph.currentRoot = currentRoot;

        uint8_t hasScale = 0;
        if (!readUint8(stream, hasScale))
            return Invalid;
        graph.hasScale = (hasScale != 0);

        if (graph.hasScale)
        {
            for (int i = 0; i < 7; ++i)
            {
                int32_t offset = 0;
                int32_t quality = 0;
                if (!readInt32(stream, offset))
                    return Invalid;
                if (!readInt32(stream, quality))
                    return Invalid;
                graph.scaleOffsets[i] = offset;
                graph.scaleDegreeQualities[i] = static_cast<ChordQuality>(quality);
            }
        }

        for (uint32_t progressionIndex = 0; progressionIndex < progressionCount; ++progressionIndex)
        {
            uint32_t nodeCount = 0;
            if (!readUint32(stream, nodeCount))
                return Invalid;

            int32_t entryIndex = -1;
            if (!readInt32(stream, entryIndex))
                return Invalid;

            SerializedProgression progression;
            progression.entryIndex = entryIndex;
            progression.nodes.reserve(nodeCount);

            for (uint32_t i = 0; i < nodeCount; ++i)
            {
                SerializedProgressionNode node;
                uint32_t type = 0;
                if (!readUint32(stream, type))
                    return Invalid;

                node.type = static_cast<SerializedProgressionNodeType>(type);

                int32_t nextIndex = -1;
                if (!readInt32(stream, nextIndex))
                    return Invalid;

                node.nextIndex = nextIndex;

                uint32_t outputsCount = 0;
                if (!readUint32(stream, outputsCount))
                    return Invalid;

                node.outputs.clear();
                node.outputs.reserve(outputsCount);
                for (uint32_t outputIndex = 0; outputIndex < outputsCount; ++outputIndex)
                {
                    int32_t outputValue = -1;
                    if (!readInt32(stream, outputValue))
                        return Invalid;

                    node.outputs.push_back(outputValue);
                }

                int32_t degree = 0;
                int32_t qualities = 0;
                int32_t accidental = 0;
                if (!readInt32(stream, degree))
                    return Invalid;
                if (!readInt32(stream, qualities))
                    return Invalid;
                if (!readInt32(stream, accidental))
                    return Invalid;

                node.chord.degree = static_cast<Degree>(degree);
                node.chord.qualities = static_cast<ChordQuality>(qualities);
                node.chord.accidental = static_cast<Accidental>(accidental);

                uint32_t duration = 0;
                if (!readUint32(stream, duration))
                    return Invalid;

                node.duration = duration;

                uint8_t scoreEnd = 0;
                if (!readUint8(stream, scoreEnd))
                    return Invalid;

                node.scoreEnd = (scoreEnd != 0);

                node.progressionChangeIndex = -1;
                node.hasScaleChange = false;
                node.motifChanges.clear();
                node.voiceChanges.clear();

                int32_t progressionChangeIndex = -1;
                if (!readInt32(stream, progressionChangeIndex))
                    return Invalid;
                node.progressionChangeIndex = progressionChangeIndex;

                uint8_t hasScaleChange = 0;
                if (!readUint8(stream, hasScaleChange))
                    return Invalid;
                node.hasScaleChange = (hasScaleChange != 0);

                if (node.hasScaleChange)
                {
                    for (int i = 0; i < 7; ++i)
                    {
                        int32_t offset = 0;
                        int32_t quality = 0;
                        if (!readInt32(stream, offset))
                            return Invalid;
                        if (!readInt32(stream, quality))
                            return Invalid;
                        node.scaleOffsets[i] = offset;
                        node.scaleDegreeQualities[i] = static_cast<ChordQuality>(quality);
                    }
                }

                uint32_t motifChangeCount = 0;
                if (!readUint32(stream, motifChangeCount))
                    return Invalid;
                node.motifChanges.reserve(motifChangeCount);
                for (uint32_t changeIndex = 0; changeIndex < motifChangeCount; ++changeIndex)
                {
                    int32_t oldIndex = -1;
                    int32_t newIndex = -1;
                    if (!readInt32(stream, oldIndex))
                        return Invalid;
                    if (!readInt32(stream, newIndex))
                        return Invalid;
                    SerializedMotifChange change;
                    change.oldMotifIndex = oldIndex;
                    change.newMotifIndex = newIndex;
                    node.motifChanges.push_back(change);
                }

                uint32_t voiceChangeCount = 0;
                if (!readUint32(stream, voiceChangeCount))
                    return Invalid;
                node.voiceChanges.reserve(voiceChangeCount);
                for (uint32_t changeIndex = 0; changeIndex < voiceChangeCount; ++changeIndex)
                {
                    int32_t oldIndex = -1;
                    int32_t newIndex = -1;
                    if (!readInt32(stream, oldIndex))
                        return Invalid;
                    if (!readInt32(stream, newIndex))
                        return Invalid;
                    SerializedVoiceChange change;
                    change.oldVoiceIndex = oldIndex;
                    change.newVoiceIndex = newIndex;
                    node.voiceChanges.push_back(change);
                }

                progression.nodes.push_back(std::move(node));
            }

            graph.progressions.push_back(std::move(progression));
        }

        uint32_t voiceCount = 0;
        if (!readUint32(stream, voiceCount))
            return Invalid;

        graph.voices.clear();
        graph.voices.reserve(voiceCount);
        for (uint32_t voiceIndex = 0; voiceIndex < voiceCount; ++voiceIndex)
        {
            SerializedVoiceDefinition voice;
            int32_t id = 0;
            if (!readInt32(stream, id))
                return Invalid;
            voice.id = id;
            if (!readString(stream, voice.name))
                return Invalid;

            uint32_t motifIndicesCount = 0;
            if (!readUint32(stream, motifIndicesCount))
                return Invalid;
            voice.motifIndices.clear();
            voice.motifIndices.reserve(motifIndicesCount);
            for (uint32_t motifIndex = 0; motifIndex < motifIndicesCount; ++motifIndex)
            {
                int32_t motifValue = -1;
                if (!readInt32(stream, motifValue))
                    return Invalid;
                voice.motifIndices.push_back(motifValue);
            }

            graph.voices.push_back(std::move(voice));
        }

        uint32_t motifCount = 0;
        if (!readUint32(stream, motifCount))
            return Invalid;

        graph.motifs.clear();
        graph.motifs.reserve(motifCount);

        for (uint32_t motifIndex = 0; motifIndex < motifCount; ++motifIndex)
        {
            SerializedMotif motif;
            int32_t harmonicAnchor = 0;
            int32_t rhythmicAnchor = 0;
            int32_t noteIntervalType = 0;
            uint32_t duration = 0;

            if (!readInt32(stream, harmonicAnchor))
                return Invalid;
            if (!readInt32(stream, rhythmicAnchor))
                return Invalid;
            if (!readInt32(stream, noteIntervalType))
                return Invalid;
            if (!readUint32(stream, duration))
                return Invalid;

            motif.harmonicAnchor = static_cast<HarmonicAnchor>(harmonicAnchor);
            motif.rhythmicAnchor = static_cast<AnchorRhythmic>(rhythmicAnchor);
            motif.noteIntervalType = static_cast<NoteIntervalType>(noteIntervalType);
            motif.duration = duration;

            uint32_t noteCount = 0;
            if (!readUint32(stream, noteCount))
                return Invalid;

            motif.notes.clear();
            motif.notes.reserve(noteCount);
            for (uint32_t noteIndex = 0; noteIndex < noteCount; ++noteIndex)
            {
                SerializedMotifNote note;
                int32_t relativeValue = 0;
                uint32_t noteDuration = 0;
                uint32_t offset = 0;

                if (!readInt32(stream, relativeValue))
                    return Invalid;
                if (!readUint32(stream, noteDuration))
                    return Invalid;
                if (!readUint32(stream, offset))
                    return Invalid;

                note.relativeValue = relativeValue;
                note.duration = noteDuration;
                note.offset = offset;
                motif.notes.push_back(note);
            }

            graph.motifs.push_back(std::move(motif));
        }

        return Success;
    }

} // namespace Dryad
