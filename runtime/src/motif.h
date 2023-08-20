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
        bool canBeTruncated; // when motif levels change, a motif can be abruptly ended (or must finish completely)
        bool canBeBent; // when the chord changes, the motif evolves to follow closely the harmony (or keeps its original harmonization)
        UInt32 maxLevel;
        Vector<Note> notes;
        void* userData;
    };
}
