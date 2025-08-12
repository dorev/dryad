#include "score.h"
#include "motif.h"
#include "scale.h"
#include "progression.h"
#include "chord.h"

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

        if (motif)
            return InvalidMotif;

        NoteValue noteAnchor = getCurrentRoot();

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
            // targeting the 4th octave by default
            NoteRelative relativeValue = motifNote->relativeValue;
            NoteRelative noteOctave = 4 + (relativeValue / 12);

            // calculating the distance & direction from the root
            Scale* scale = getCurrentScale();
            NoteRelative noteOffset = scale->noteOffsets.degrees[relativeValue % static_cast<int>(Degree::limit)];

            // final note value
            NoteValue root = getCurrentRoot();
            noteValue = notes[root][noteOctave] + noteOffset;
        }
        break;

        case HarmonicAnchor::chord:
        {
            Chord chord = getCurrentChord();

            // find the offsets of the chord notes based on its qualities
            // if the qualities == default, use the scale degrees
            // 


            // we have to find the chord notes in the scale corresponding to that degree
            // the we have to apply the additional accidental and alterations
            // then we scroll through the notes of that chord, which might have more than 3 notes, based on the chord type

            // CATCH UP HERE
        }
        break;

        default:
            return NotImplemented;
        }


        NoteInstance* noteInstance = graph->create<NoteInstance>(noteValue, motifNote->duration);
        graph->link(noteInstance, this);
        graph->link(noteInstance, motifNote);


        return NotImplemented;
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
