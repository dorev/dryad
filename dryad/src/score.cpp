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
        return getScore()->getCurrentRoot();
    }

    Scale* ScoreFrame::getCurrentScale()
    {
        return getScore()->getCurrentScale();
    }

    Chord ScoreFrame::getCurrentChord()
    {
        Score* score = getScore();
        if (!score || !score->getCurrentProgression())
            return Chord();

        Progression* progression = score->getCurrentProgression();
        ProgressionChord* chord = progression ? progression->currentProgressionChord : nullptr;
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
        : m_currentRoot(C)
        , m_currentProgression(nullptr)
        , m_currentScale(nullptr)
        , m_voices()
        , m_frames()
        , m_progressionCursor(nullptr)
        , m_remainingChordDuration(0)
    {
    }

    Voice* Score::addVoice(int id, String name)
    {
        Voice* voice = create<Voice>(id, name);

        if (voice)
            m_voices.insert(voice);

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

        if (m_currentProgression && !m_currentProgression->currentProgressionChord)
        {
            if (m_currentProgression->entryNode)
                m_currentProgression->currentProgressionChord =
                    m_currentProgression->entryNode->get<ProgressionChord>();
        }

        // For every motif of each voice, generate instances until the total committed
        // duration is reached
        for (Voice* voice : m_voices)
            voice->generateUntil(commitEndPosition);

        // Mark frames up to the committed duration as committed
        for (ScoreFrame* f : m_frames)
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

        if (m_currentProgression)
            {
            if (!m_progressionCursor)
                m_progressionCursor = m_currentProgression->entryNode;

                // Search for the next progression node to handle
            while (m_progressionCursor)
                {
                    // Handle progression events
                if (ProgressionEvent* eventNode = m_progressionCursor->get<ProgressionEvent>())
                    {
                        if (eventNode->scaleChange)
                        m_currentScale = eventNode->scaleChange;

                        if (eventNode->progressionChange)
                        {
                        setCurrentProgression(eventNode->progressionChange);
                            continue;
                        }

                    m_progressionCursor = eventNode->next;
                        continue;
                    }

                    // Handle progression switch sequences
                if (ProgressionSwitchSequence* switchNode = m_progressionCursor->get<ProgressionSwitchSequence>())
                    {
                        if (!switchNode->outputs.empty())
                        {
                            if (switchNode->nextOutputIndex < 0 || switchNode->nextOutputIndex >= static_cast<int>(switchNode->outputs.size()))
                                switchNode->nextOutputIndex = 0;

                        m_progressionCursor = switchNode->outputs[switchNode->nextOutputIndex];
                            switchNode->nextOutputIndex = (switchNode->nextOutputIndex + 1) % static_cast<int>(switchNode->outputs.size());
                        }
                        else
                        {
                        m_progressionCursor = switchNode->next;
                        }

                        continue;
                    }

                chordNode = m_progressionCursor->get<ProgressionChord>();
                    if (chordNode)
                    {
                    if (m_currentProgression->currentProgressionChord != chordNode)
                        {
                        m_currentProgression->currentProgressionChord = chordNode;
                        m_remainingChordDuration = chordNode->duration;
                        }

                    if (m_remainingChordDuration == 0)
                        {
                        m_progressionCursor = chordNode->next ? chordNode->next : m_currentProgression->entryNode;
                            chordNode = nullptr;
                            continue;
                        }

                    stepDuration = remainingDuration < m_remainingChordDuration ? remainingDuration : m_remainingChordDuration;
                    }

                    break;
                }
            }

            Error error = commitDuration(stepDuration);
            if (error)
                return error;

            CollectCommittedEvents(m_frames, outEvents);

            if (chordNode)
            {
            if (stepDuration >= m_remainingChordDuration)
                m_remainingChordDuration = 0;
                else
                m_remainingChordDuration -= stepDuration;

            if (m_remainingChordDuration == 0)
                m_progressionCursor = chordNode->next ? chordNode->next : m_currentProgression->entryNode;
            }

            remainingDuration -= stepDuration;
        }

        return Success;
    }

    Error Score::dump(SerializedScore& serializedScore)
    {
        serializedScore.voices.clear();

        for (Voice* voice : m_voices)
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
                m_frames.insert(frame);
            }
        }

        return frame;
    }

    ScoreFrame* Score::findFrameAtPosition(Time position)
    {
        // Creating a dummy frame with the searched position to leverage std::find
        ScoreFrame dummy_frame(position);

        auto it = m_frames.find(&dummy_frame);

        if (it == m_frames.end())
            return nullptr;

        return *it;
    }

    ScoreFrame* Score::findLastCommittedFrame()
    {
        ScoreFrame* frame = nullptr;

        for (auto it = m_frames.rbegin(); it != m_frames.rend(); ++it)
        {
            if ((*it)->isCommitted)
            {
                frame = *it;
                break;
            }
        }

        return frame;
    }

    NoteValue Score::getCurrentRoot() const
    {
        return m_currentRoot;
    }

    void Score::setCurrentRoot(NoteValue root)
    {
        m_currentRoot = root;
    }

    Scale* Score::getCurrentScale() const
    {
        return m_currentScale;
    }

    void Score::setCurrentScale(Scale* scale)
    {
        m_currentScale = scale;
    }

    Progression* Score::getCurrentProgression() const
    {
        return m_currentProgression;
    }

    void Score::setCurrentProgression(Progression* progression)
    {
        m_currentProgression = progression;
        m_progressionCursor = progression ? progression->entryNode : nullptr;
        m_remainingChordDuration = 0;
    }

    void Score::resetTimeline()
    {
        Vector<ScoreFrame*> framesToDestroy;
        forEachEdge<ScoreFrame>([&](ScoreFrame* frame)
            {
                framesToDestroy.push_back(frame);
            });

        Vector<NoteInstance*> noteInstancesToDestroy;
        forEachEdge<NoteInstance>([&](NoteInstance* noteInstance)
            {
                noteInstancesToDestroy.push_back(noteInstance);
            });

        Vector<MotifInstance*> motifInstancesToDestroy;
        forEachEdge<MotifInstance>([&](MotifInstance* motifInstance)
            {
                motifInstancesToDestroy.push_back(motifInstance);
            });

        for (NoteInstance* noteInstance : noteInstancesToDestroy)
            destroy(noteInstance);
        for (MotifInstance* motifInstance : motifInstancesToDestroy)
            destroy(motifInstance);
        for (ScoreFrame* frame : framesToDestroy)
            destroy(frame);

        m_frames.clear();
        m_progressionCursor = nullptr;
        m_remainingChordDuration = 0;
        if (m_currentProgression)
            m_currentProgression->currentProgressionChord = nullptr;
    }

} // namespace Dryad
