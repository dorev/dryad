#include "score.h"
#include "motif.h"
#include "scale.h"
#include "harmony.h"
#include "progression.h"
#include "chord.h"
#include "constants.h"
#include "serialize.h"

namespace Dryad
{

    ScoreFrame::ScoreFrame(Time position)
        : position(position)
        , isCommitted(false)
        , isEmitted(false)
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
        , progressionCursor(nullptr)
        , remainingChordDuration(0)
    {
    }

    Voice* Score::addVoice(int id, String name)
    {
        Voice* voice = create<Voice>(id, name);

        if (voice)
            voices.insert(voice);

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
                ScoreFrame* frame = getOrCreateFrame(position + note->offset);

                error = frame->addMotifNote(note);
                return error != Success;
            });

        return error;
    }

    Error Score::commitDuration(Time duration)
    {
        ScoreFrame* frame = findLastCommittedFrame();

        if (!frame)
            frame = getOrCreateFrame(0);

        Time commitEndPosition = frame->position + duration;

        if (currentProgression && !currentProgression->currentProgressionChord)
        {
            if (currentProgression->entryNode)
                currentProgression->currentProgressionChord =
                    currentProgression->entryNode->get<ProgressionChord>();
        }

        // For every motif of each voice, generate instances until the total committed
        // duration is reached
        for (Voice* voice : voices)
            voice->generateUntil(commitEndPosition);

        // Mark frames up to the committed duration as committed
        for (ScoreFrame* f : frames)
        {
            if (f->position <= commitEndPosition)
                f->isCommitted = true;
        }

        return Success;
    }

    static int FindVoiceId(NoteInstance* instance)
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
                onEvent.position = frame->position;
                onEvent.noteValue = noteInstance->value;
                onEvent.noteDuration = noteInstance->duration;
                onEvent.voiceId = FindVoiceId(noteInstance);
                outEvents.push_back(onEvent);

                ScoreEvent offEvent;
                offEvent.type = ScoreEventType::NoteOff;
                offEvent.position = frame->position + noteInstance->duration;
                offEvent.noteValue = noteInstance->value;
                offEvent.noteDuration = noteInstance->duration;
                offEvent.voiceId = onEvent.voiceId;
                outEvents.push_back(offEvent);
            });
    }

    static void CollectCommittedEvents(const Set<ScoreFrame*, ScoreFrame::CompareByPosition>& frames,
        Vector<ScoreEvent>& outEvents)
    {
        for (ScoreFrame* frame : frames)
        {
            if (!frame->isCommitted)
                continue;

            if (!frame->isEmitted)
            {
                AppendFrameEvents(frame, outEvents);
                frame->isEmitted = true;
            }
        }
    }

    Error Score::tick(Time delta, Vector<ScoreEvent>& outEvents)
    {
        if (delta == 0)
            return Success;

        Time remainingDuration = delta;

        while (remainingDuration > 0)
        {
            Time stepDuration = remainingDuration;
            ProgressionChord* chordNode = nullptr;

            if (currentProgression)
            {
                if (!progressionCursor)
                    progressionCursor = currentProgression->entryNode;

                // Search for the next progression node to handle
                while (progressionCursor)
                {
                    // Handle progression events
                    if (ProgressionEvent* eventNode = progressionCursor->get<ProgressionEvent>())
                    {
                        if (eventNode->scaleChange)
                            currentScale = eventNode->scaleChange;

                        if (eventNode->progressionChange)
                        {
                            currentProgression = eventNode->progressionChange;
                            progressionCursor = currentProgression ? currentProgression->entryNode : nullptr;
                            remainingChordDuration = 0;
                            continue;
                        }

                        progressionCursor = eventNode->next;
                        continue;
                    }

                    // Handle progression switch sequences
                    if (ProgressionSwitchSequence* switchNode = progressionCursor->get<ProgressionSwitchSequence>())
                    {
                        if (!switchNode->outputs.empty())
                        {
                            if (switchNode->nextOutputIndex < 0 || switchNode->nextOutputIndex >= static_cast<int>(switchNode->outputs.size()))
                                switchNode->nextOutputIndex = 0;

                            progressionCursor = switchNode->outputs[switchNode->nextOutputIndex];
                            switchNode->nextOutputIndex = (switchNode->nextOutputIndex + 1) % static_cast<int>(switchNode->outputs.size());
                        }
                        else
                        {
                            progressionCursor = switchNode->next;
                        }

                        continue;
                    }

                    chordNode = progressionCursor->get<ProgressionChord>();
                    if (chordNode)
                    {
                        if (currentProgression->currentProgressionChord != chordNode)
                        {
                            currentProgression->currentProgressionChord = chordNode;
                            remainingChordDuration = chordNode->duration;
                        }

                        if (remainingChordDuration == 0)
                        {
                            progressionCursor = chordNode->next ? chordNode->next : currentProgression->entryNode;
                            chordNode = nullptr;
                            continue;
                        }

                        stepDuration = remainingDuration < remainingChordDuration ? remainingDuration : remainingChordDuration;
                    }

                    break;
                }
            }

            Error error = commitDuration(stepDuration);
            if (error)
                return error;

            CollectCommittedEvents(frames, outEvents);

            if (chordNode)
            {
                if (stepDuration >= remainingChordDuration)
                    remainingChordDuration = 0;
                else
                    remainingChordDuration -= stepDuration;

                if (remainingChordDuration == 0)
                    progressionCursor = chordNode->next ? chordNode->next : currentProgression->entryNode;
            }

            remainingDuration -= stepDuration;
        }

        return Success;
    }

    Error Score::dump(SerializedScore& serializedScore)
    {
        serializedScore.voices.clear();

        for (Voice* voice : voices)
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
                                if (!frame || !frame->isCommitted)
                                    return;

                                SerializedNote serializedNote;
                                serializedNote.position = frame->position;
                                serializedNote.duration = instance->duration;
                                serializedNote.noteValue = instance->value;
                                serializedVoice.notes.push_back(serializedNote);
                            });
                    });
            }

            serializedScore.voices.push_back(std::move(serializedVoice));
        }

        return Success;
    }

    ScoreFrame* Score::getOrCreateFrame(Time position)
    {
        ScoreFrame* frame = findFrameAtPosition(position);

        if (!frame)
        {
            frame = create<ScoreFrame>(position);

            if (!frame)
            {
                DRYAD_FATAL("Unable to create a new frame at position %d.", position);

            }
            else
            {
                frames.insert(frame);
            }
        }

        return frame;
    }

    ScoreFrame* Score::findFrameAtPosition(Time position)
    {
        // Creating a dummy frame with the searched position to leverage std::find
        ScoreFrame dummy_frame(position);

        auto it = frames.find(&dummy_frame);

        if (it == frames.end())
            return nullptr;

        return *it;
    }

    ScoreFrame* Score::findLastCommittedFrame()
    {
        ScoreFrame* frame = nullptr;

        for (auto it = frames.rbegin(); it != frames.rend(); ++it)
        {
            if ((*it)->isCommitted)
            {
                frame = *it;
                break;
            }
        }

        return frame;
    }

} // namespace Dryad
