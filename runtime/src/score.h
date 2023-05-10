#pragma once

#include "types.h"
#include "note.h"

namespace Dryad
{
    class ScoreCachedInfo
    {
        Time startTime;
        Position committedDuration;
        Position generatedDuration;
        HarmonicContextPtr currentHarmonicContext;
    };

    class Score
    {
        ScoreCachedInfo info;
        Vector<Note> notes;
    };
}
