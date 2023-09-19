#pragma once

#include "types.h"
#include "containers.h"
#include "note.h"

namespace Dryad
{
    enum class MotifMode
    {
        Diatonic,
        Chromatic,
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
        ScoreTime rhythmicAlignment;
        bool alignRhythmToNode;
        HarmonicAnchor harmonicAnchor;
        bool canBeTruncated; // when motif levels change, a motif can be abruptly ended (or must finish completely)
        bool canBeBent; // when the chord changes, the motif evolves to follow closely the harmony (or keeps its original harmonization)
        UInt32 maxLevel;
        Vector<Note> notes;
        void* userData;
    };
}
