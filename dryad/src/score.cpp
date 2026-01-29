#include "score.h"
#include "motif.h"
#include "scale.h"
#include "progression.h"
#include "chord.h"
#include "constants.h"
#include "serialize.h"

namespace Dryad
{

    ScoreFrame::ScoreFrame(Time relativePosition)
        : relativePosition(relativePosition)
        , committed(false)
        , emitted(false)
    {
    }

    Score* ScoreFrame::getScore()
    {
        return static_cast<Score*>(graph);
    }

    NoteValue ScoreFrame::getCurrentRoot()
    {
        return getScore()->currentRoot;
    }

    Scale* ScoreFrame::getCurrentScale()
    {
        return getScore()->currentScale;
    }

    Chord ScoreFrame::getCurrentChord()
    {
        Score* score = getScore();
        if (!score || !score->currentProgression)
            return Chord();

        ProgressionChord* chord = score->currentProgression->currentProgressionChord;
        if (!chord)
            return Chord();

        return chord->chord;
    }

    Error ScoreFrame::addMotifNote(MotifNote* motifNote)
    {
        if (!motifNote)
            return InvalidMotifNote;

        Motif* motif = motifNote->findFirstEdge<Motif>();

        if (!motif)
            return InvalidMotif;

        // we have to find the current scale to know the root
        // then we check the harmonic anchor of the motif
        // if it's the scale...
        // if it's the chord...
        // TODO: at some point we will also have to take the voice range in consideration
        NoteValue noteValue = InvalidValue;

        switch (motif->harmonicAnchor)
        {
        case HarmonicAnchor::Scale:
        {
            // Interpret the relative value as a scale degree offset from the
            // current root. We wrap around every octave using the scale degree
            // count and adjust the octave accordingly.
            int noteIndex = motifNote->relativeValue;
            int octaveOffset = noteIndex / DegreesPerOctave;
            int degreeIndex = noteIndex % DegreesPerOctave;
            if (degreeIndex < 0)
            {
                degreeIndex += DegreesPerOctave;
                --octaveOffset;
            }

            Scale* scale = getCurrentScale();
            if (!scale)
                return InvalidScale;

            NoteRelative degreeOffset = scale->noteOffsets.degrees[degreeIndex];

            int octaveIndex = MiddleOctave + octaveOffset;
            if (octaveIndex < 0 || octaveIndex >= OctaveLimit)
                return InvalidMotifNote;

            NoteValue root = getCurrentRoot();
            noteValue = notes[root][octaveIndex] + degreeOffset;
        }
        break;

        case HarmonicAnchor::Chord:
        {
            // Use the chord's root as the starting scale degree and interpret
            // the motif's relative value as additional scale degrees from that
            // point. This allows motif notes to wander outside the chord while
            // remaining on the scale.
            Chord chord = getCurrentChord();
            if (chord.degree == Degree::Invalid)
                return InvalidDegree;

            int step = static_cast<int>(chord.degree) + motifNote->relativeValue;
            int octaveOffset = step / DegreesPerOctave;
            int degreeIndex = step % DegreesPerOctave;
            if (degreeIndex < 0)
            {
                degreeIndex += DegreesPerOctave;
                --octaveOffset;
            }

            Scale* scale = getCurrentScale();
            if (!scale)
                return InvalidScale;

            NoteRelative degreeOffset = scale->noteOffsets.degrees[degreeIndex];

            NoteRelative accidentalOffset = 0;
            if (chord.accidental == Accidental::Flat)
                accidentalOffset = -1;
            else if (chord.accidental == Accidental::Sharp)
                accidentalOffset = 1;

            int octaveIndex = MiddleOctave + octaveOffset;
            if (octaveIndex < 0 || octaveIndex >= OctaveLimit)
                return InvalidMotifNote;

            NoteValue root = getCurrentRoot();
            noteValue = notes[root][octaveIndex] + degreeOffset + accidentalOffset;
        }
        break;

        default:
            return NotImplemented;
        }

        NoteInstance* noteInstance = graph->create<NoteInstance>(noteValue, motifNote->duration);
        graph->link(noteInstance, this);
        graph->link(noteInstance, motifNote);

        return Success;
    }

    Score::Score()
        : currentRoot(C)
        , currentProgression(nullptr)
        , currentScale(nullptr)
        , currentProgressionNode(nullptr)
        , currentChordRemaining(0)
    {
    }

    Voice* Score::addVoice(int id, String name)
    {
        Voice* voice = create<Voice>(id, name);

        if (voice)
            cachedVoices.insert(voice);

        return voice;
    }

    Error Score::addMotifInstance(Voice* voice, Motif* motif, Time position, MotifInstance*& instance)
    {
        if (!voice)
            return InvalidVoice;

        if (!motif)
            return InvalidMotif;

        if (position == Invalid)
            return InvalidPosition;

        instance = create<MotifInstance>(position);
        if (!instance)
            return InvalidMotifInstance;

        // Link the new instance with its motif and voice so it can be queried later
        link(instance, motif);
        link(instance, voice);

        // TODO: we have to identify of there is a rythmic anchor to respect for the motif!!

        // Add each note of the motif in already existing or new frames

        Error error = Success;
        motif->forEachEdgeBreakable<MotifNote>([&](MotifNote* note) -> bool
            {
                ScoreFrame* frame = getOrCreateFrame(position + note->relativePosition);

                error = frame->addMotifNote(note);
                return error != Success;
            });

        return error;
    }

    Error Score::commit(Time durationToCommit)
    {
        ScoreFrame* frame = findLastCommittedFrame();

        if (!frame)
            frame = getOrCreateFrame(0);

        Time relativePosition = frame->relativePosition + durationToCommit;

        if (currentProgression && !currentProgression->currentProgressionChord)
        {
            if (currentProgression->entryNode)
                currentProgression->currentProgressionChord =
                    currentProgression->entryNode->get<ProgressionChord>();
        }

        // For every motif of each voice, generate instances until the total committed
        // duration is reached
        for (Voice* voice : cachedVoices)
            voice->generateUntil(relativePosition);

        // Mark frames up to the committed duration as committed
        for (ScoreFrame* f : cachedFrames)
        {
            if (f->relativePosition <= relativePosition)
                f->committed = true;
        }

        return Success;
    }

    static int ResolveVoiceId(NoteInstance* instance)
    {
        if (!instance)
            return -1;

        MotifNote* motifNote = instance->findFirstEdge<MotifNote>();
        if (!motifNote)
            return -1;

        Motif* motif = motifNote->findFirstEdge<Motif>();
        if (!motif)
            return -1;

        Voice* voice = motif->findFirstEdge<Voice>();
        if (!voice)
            return -1;

        return voice->id;
    }

    static void AppendFrameEvents(ScoreFrame* frame, Vector<ScoreEvent>& outEvents)
    {
        if (!frame)
            return;

        frame->forEachEdge<NoteInstance>([&](NoteInstance* noteInstance)
            {
                ScoreEvent onEvent;
                onEvent.type = ScoreEventType::NoteOn;
                onEvent.time = frame->relativePosition;
                onEvent.value = noteInstance->value;
                onEvent.duration = noteInstance->duration;
                onEvent.voiceId = ResolveVoiceId(noteInstance);
                outEvents.push_back(onEvent);

                ScoreEvent offEvent;
                offEvent.type = ScoreEventType::NoteOff;
                offEvent.time = frame->relativePosition + noteInstance->duration;
                offEvent.value = noteInstance->value;
                offEvent.duration = noteInstance->duration;
                offEvent.voiceId = onEvent.voiceId;
                outEvents.push_back(offEvent);
            });
    }

    static void CollectCommittedEvents(const Set<ScoreFrame*, ScoreFrame::CompareByPosition>& frames,
        Vector<ScoreEvent>& outEvents)
    {
        for (ScoreFrame* frame : frames)
        {
            if (!frame->committed)
                continue;

            if (!frame->emitted)
            {
                AppendFrameEvents(frame, outEvents);
                frame->emitted = true;
            }
        }
    }

    Error Score::tick(Time durationToCommit, Vector<ScoreEvent>& outEvents)
    {
        if (durationToCommit == 0)
            return Success;

        Time remaining = durationToCommit;

        while (remaining > 0)
        {
            Time segment = remaining;
            ProgressionChord* chordNode = nullptr;

            if (currentProgression)
            {
                if (!currentProgressionNode)
                    currentProgressionNode = currentProgression->entryNode;

                // Search for the next progression node to handle
                while (currentProgressionNode)
                {
                    // Handle progression events
                    if (ProgressionEvent* eventNode = currentProgressionNode->get<ProgressionEvent>())
                    {
                        if (eventNode->scaleChange)
                            currentScale = eventNode->scaleChange;

                        if (eventNode->progressionChange)
                        {
                            currentProgression = eventNode->progressionChange;
                            currentProgressionNode = currentProgression ? currentProgression->entryNode : nullptr;
                            currentChordRemaining = 0;
                            continue;
                        }

                        currentProgressionNode = eventNode->next;
                        continue;
                    }

                    // Handle progression switch sequences
                    if (ProgressionSwitchSequence* switch = currentProgressionNode->get<ProgressionSwitchSequence>())
                    {
                        if (!switch->outputs.empty())
                        {
                            if (switch->outputIndex < 0 || switch->outputIndex >= static_cast<int>(switch->outputs.size()))
                                switch->outputIndex = 0;

                            currentProgressionNode = switch->outputs[switch->outputIndex];
                            switch->outputIndex = (switch->outputIndex + 1) % static_cast<int>(switch->outputs.size());
                        }
                        else
                        {
                            currentProgressionNode = switch->next;
                        }

                        continue;
                    }

                    chordNode = currentProgressionNode->get<ProgressionChord>();
                    if (chordNode)
                    {
                        if (currentProgression->currentProgressionChord != chordNode)
                        {
                            currentProgression->currentProgressionChord = chordNode;
                            currentChordRemaining = chordNode->duration;
                        }

                        if (currentChordRemaining == 0)
                        {
                            currentProgressionNode = chordNode->next ? chordNode->next : currentProgression->entryNode;
                            chordNode = nullptr;
                            continue;
                        }

                        segment = remaining < currentChordRemaining ? remaining : currentChordRemaining;
                    }

                    break;
                }
            }

            Error error = commit(segment);
            if (error)
                return error;

            CollectCommittedEvents(cachedFrames, outEvents);

            if (chordNode)
            {
                if (segment >= currentChordRemaining)
                    currentChordRemaining = 0;
                else
                    currentChordRemaining -= segment;

                if (currentChordRemaining == 0)
                    currentProgressionNode = chordNode->next ? chordNode->next : currentProgression->entryNode;
            }

            remaining -= segment;
        }

        return Success;
    }

    Error Score::dump(SerializedScore& serializedScore)
    {
        serializedScore.cachedVoices.clear();

        for (Voice* voice : cachedVoices)
        {
            SerializedVoice serializedVoice;
            serializedVoice.name = voice->name;
            serializedVoice.id = voice->id;

            for (Motif* motif : voice->motifs)
            {
                motif->forEachEdge<MotifNote>([&](MotifNote* note)
                    {
                        note->forEachEdge<NoteInstance>([&](NoteInstance* instance)
                            {
                                ScoreFrame* frame = instance->findFirstEdge<ScoreFrame>();
                                if (!frame || !frame->committed)
                                    return;

                                SerializedNote serializedNote;
                                serializedNote.relativePosition = frame->relativePosition;
                                serializedNote.duration = instance->duration;
                                serializedNote.value = instance->value;
                                serializedVoice.notes.push_back(serializedNote);
                            });
                    });
            }

            serializedScore.cachedVoices.push_back(std::move(serializedVoice));
        }

        return Success;
    }

    ScoreFrame* Score::getOrCreateFrame(Time relativePosition)
    {
        ScoreFrame* frame = findFrameAtPosition(relativePosition);

        if (!frame)
        {
            frame = create<ScoreFrame>(relativePosition);

            if (!frame)
            {
                DRYAD_FATAL("Unable to create a new frame at position %d.", relativePosition);

            }
            else
            {
                cachedFrames.insert(frame);
            }
        }

        return frame;
    }

    ScoreFrame* Score::findFrameAtPosition(Time relativePosition)
    {
        // Creating a dummy frame with the searched position to leverage std::find
        ScoreFrame dummy_frame(relativePosition);

        auto it = cachedFrames.find(&dummy_frame);

        if (it == cachedFrames.end())
            return nullptr;

        return *it;
    }

    ScoreFrame* Score::findLastCommittedFrame()
    {
        ScoreFrame* frame = nullptr;

        for (auto it = cachedFrames.rbegin(); it != cachedFrames.rend(); ++it)
        {
            if ((*it)->committed)
            {
                frame = *it;
                break;
            }
        }

        return frame;
    }

} // namespace Dryad
