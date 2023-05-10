#pragma once

#include "types.h"
#include "identification.h"
#include "note.h"

namespace Dryad
{
    enum MotifMode : UInt8
    {
        Diatonic,
        Chromatic,
    };

    enum RythmicAnchor : UInt8
    {
        Anywhere,
        OnAnyBeat,
        OnFirstBeat,
        OnMiddleBeat,
        OnLastBeat,
    };

    enum HarmonicAnchor : UInt8
    {
        OnScale,
        OnChord,
    };

    class Motif : public Named
    {
        MotifMode mode;
        RythmicAnchor rythmicAnchor;
        HarmonicAnchor harmonicAnchor;
        UInt8 maxLevel;
        Vector<Note> notes;
        void* userData;
    };
}
