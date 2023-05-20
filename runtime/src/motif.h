#pragma once

#include "types.h"
#include "note.h"

namespace Dryad
{
    enum class MotifMode
    {
        Diatonic,
        Chromatic,
    };

    enum class RythmicAnchor
    {
        Anywhere,
        OnAnyBeat,
        OnFirstBeat,
        OnMiddleBeat,
        OnLastBeat,
    };

    enum class HarmonicAnchor
    {
        OnScale,
        OnChord,
    };

    class Motif
    {
    public:
        MotifMode mode;
        RythmicAnchor rythmicAnchor;
        HarmonicAnchor harmonicAnchor;
        UInt8 maxLevel;
        Vector<Note> notes;
        void* userData;
    };
}
