#include "score.h"
#include "motif.h"
#include "scale.h"
#include "progression.h"
#include "chord.h"
#include "constants.h"

namespace Dryad
{

    ScoreFrame::ScoreFrame(Time relativePosition)
        : relativePosition(relativePosition)
        , committed(false)
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
        return getScore()->currentProgression->currentProgressionChord->chord;
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
        case HarmonicAnchor::scale:
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

        case HarmonicAnchor::chord:
        {
            // Use the chord's root as the starting scale degree and interpret
            // the motif's relative value as additional scale degrees from that
            // point. This allows motif notes to wander outside the chord while
            // remaining on the scale.
            Chord chord = getCurrentChord();
            if (chord.degree == Degree::invalid)
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
            if (chord.accidental == Accidental::flat)
                accidentalOffset = -1;
            else if (chord.accidental == Accidental::sharp)
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

        // For every motif of each voice, generate instances until the total committed
        // duration is reached
        for (Voice* voice : cachedVoices)
            voice->generateUntil(relativePosition);

        // Mark frames up to the committed duration as committed
        for (ScoreFrame* f : cached_frames)
        {
            if (f->relativePosition <= relativePosition)
                f->committed = true;
        }

        return Success;
    }

    Error Score::dump(SerializedScore& serializedScore)
    {
        return NotImplemented;
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
                cached_frames.insert(frame);
            }
        }

        return frame;
    }

    ScoreFrame* Score::findFrameAtPosition(Time relativePosition)
    {
        // Creating a dummy frame with the searched position to leverage std::find
        ScoreFrame dummy_frame(relativePosition);

        auto it = cached_frames.find(&dummy_frame);

        if (it == cached_frames.end())
            return nullptr;

        return *it;
    }

    ScoreFrame* Score::findLastCommittedFrame()
    {
        ScoreFrame* frame = nullptr;

        for (auto it = cached_frames.rbegin(); it != cached_frames.rend(); ++it)
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
