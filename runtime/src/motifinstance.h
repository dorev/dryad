#pragma once

#include "types.h"
#include "containers.h"
#include "note.h"
#include "result.h"
#include "motif.h"
#include "constants.h"

namespace Dryad
{
    class ScoreFrame;

    class MotifInstance
    {
    public:
        MotifInstance(const Motif* motif, ScoreTime startTime)
            : m_Motif(motif)
            , m_StartTime(startTime)
            , m_Notes()
        {
        }

        Result UpdateNotes(NoteValue referenceNote)
        {
            if (m_Motif == nullptr)
            {
                return Result::InvalidMotif;
            }
            m_Notes.Clear();
            m_Notes.Reserve(m_Motif->notes.Size());
            for (const Note& note : m_Motif->notes)
            {
                Int32 noteValue = referenceNote + note.relativeValue;
                if (noteValue < 0 || noteValue > NoteValueMax)
                {
                    return Result::InvalidNote;
                }
                m_Notes.PushBack(Note(static_cast<NoteValue>(noteValue), this, nullptr, m_StartTime + note.position, note.duration));
            }
            return Result::Success;
        }

        const Motif* GetMotif() const
        {
            return m_Motif;
        }

        const Motif* m_Motif;
        ScoreTime m_StartTime;
        ScoreFrame* m_StartFrame;
        Vector<Note> m_Notes;
    };
}
