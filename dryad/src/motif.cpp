#include "motif.h"
#include "score.h"
#include "voice.h"

namespace Dryad
{

    MotifNote::MotifNote(Motif* motif, NoteRelative relativeValue, Time duration, Time relativePosition)
        : relativeValue(relativeValue)
        , duration(duration)
        , relativePosition(relativePosition)
    {
    }

    NoteInstance::NoteInstance(NoteValue value, Time duration)
        : value(value)
        , duration(duration)
    {
    }

    MotifNote* Motif::addNote(NoteRelative relativeValue, Time duration, Time relativePosition)
    {
        MotifNote* motifNote = graph->create<MotifNote>(this, relativeValue, duration, relativePosition);

        if (!motifNote)
            return nullptr;

        graph->link(this, motifNote);

        updateDuration();

        return motifNote;
    }

    void Motif::updateDuration()
    {
        Time calculatedDuration = 0;
        forEachEdge<MotifNote>([&](MotifNote* note)
            {
                Time noteEnd = note->relativePosition + note->duration;

                if (noteEnd > calculatedDuration)
                    calculatedDuration = noteEnd;
            });

        if (calculatedDuration > duration)
            duration = calculatedDuration;
    }

    bool Motif::destroyNote(MotifNote* motifNoteToRemove)
    {
        bool motifNoteOwned = false;

        // this for loop breaks when returning true
        forEachEdgeBreakable<MotifNote>([&](MotifNote* motifNote) -> bool
            {
                return motifNoteOwned = (motifNoteToRemove == motifNote);
            });

        if (!motifNoteOwned)
            return false;

        // Remove all uncommitted notes instances bound to motifNoteToRemove
        forEachEdge<NoteInstance>([&](NoteInstance* note)
            {
                ScoreFrame* noteFrame = note->findFirstEdge<ScoreFrame>();

                if (noteFrame && !noteFrame->committed)
                    graph->destroy(note);
                else
                    note->removeEdge(motifNoteToRemove);
            });

        return graph->destroy(motifNoteToRemove);
    }

    MotifInstance* Motif::getLastInstance()
    {
        MotifInstance* lastInstance = nullptr;
        Time lastInstanceEnd = 0;

        forEachEdge<MotifInstance>([&](MotifInstance* instance)
            {
                Time instanceEnd = instance->position + duration;
                if (instanceEnd > lastInstanceEnd)
                {
                    lastInstanceEnd = instanceEnd;
                    lastInstance = instance;
                }
            });

        return lastInstance;
    }

    Error Motif::getInstancesEndTime(Voice* voice, Time& time)
    {
        if (!voice)
            return InvalidVoice;

        if (!graph->contains(voice))
            return NodeNotInGraph;

        time = 0;
        bool foundInstance = false;

        forEachEdge<MotifInstance>([&](MotifInstance* instance)
            {
                Voice* instanceVoice = instance->findFirstEdge<Voice>();
                if (instanceVoice != voice)
                    return;

                Time end = instance->getEndTime();
                if (end == Invalid)
                {
                    time = Invalid;
                    return;
                }

                if (end > time)
                    time = end;

                foundInstance = true;
            });

        if (!foundInstance)
            time = 0;

        if (time == Invalid)
            return InvalidMotifInstance;

        return Success;
    }

    MotifInstance::MotifInstance(Time position)
        : position(position)
    {
        if (position == Invalid)
            DRYAD_ERROR("Motif instance constructed with invalid position.");
    }

    Time MotifInstance::getEndTime()
    {
        Motif* motif = findFirstEdge<Motif>();

        if (!motif)
            return Invalid;

        return position + motif->duration;
    }

} // namespace Dryad
