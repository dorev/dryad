#pragma once

#include "types.h"
#include "containers.h"
#include "note.h"

namespace Dryad
{
    enum class MotifMode
    {
        // Motif notes is restricted to the scale notes
        Diatonic,
        // Motif notes can be any interval from the scales root
        Chromatic,
    };

    enum class HarmonicAnchor
    {
        // Bind the motifs harmonization to the scale or to the current chord
        OnScale,
        OnChord,
    };

    class Motif
    {
    public:
        MotifMode mode;
        HarmonicAnchor harmonicAnchor;

        // Rythmic alignment
        // The minimal beat fraction the motif has to clip to
        ScoreTime rhythmicAlignment;

        // AlignRythmToNode
        // Force a motif to always start on the beginning of a node when it starts for the first time
        bool alignRhythmToNode;

        // CanBeTruncated
        // When motif levels change, a motif can be abruptly ended, or must finish completely
        // This is relevant for longer motifs to be sure they don't overextend on the score, while
        // it makes sense to let shorter motifs finish
        bool canBeTruncated;

        // CanBeBent
        // When the chord changes, the motif evolves to follow closely the harmony. For some motifs
        // extending over multiple chord or scale changes, we might want to anchor its voicing on the
        // chord it started rather than changing its notes relatively
        bool canBeBent;

        UInt32 maxConcurrentInstances;
        Vector<Note> notes;
        void* userData;
    };
}
