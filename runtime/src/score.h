#pragma once

#include "types.h"
#include "time.h"
#include "note.h"

namespace Dryad
{
    class HarmonicContext;

    struct ScoreInfo
    {
        Time startTime;
        Position committedDuration;
        Position generatedDuration;
        HarmonicContext* currentHarmonicContext;
    };

    class Score
    {
    public:
        void Reset(Time startTime);

    private:
        ScoreInfo info;
        Vector<Note> notes;
    };
}
