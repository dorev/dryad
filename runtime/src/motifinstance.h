#pragma once

#include "types.h"
#include "note.h"

namespace Dryad
{
    class Motif;

    class MotifInstance
    {
    public:
        const Motif* motif;
        ScoreTime startPosition;
        Vector<Note*> notes;
        // When a frame is destroyed, it must make sure to clean notes here too
    };
}
