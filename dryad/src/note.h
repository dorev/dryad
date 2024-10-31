#pragma once

#include "types.h"

namespace Dryad
{
    class Node;
    class MotifInstance;
    class ScoreFrame;

    constexpr NoteVelocity DefaultVelocity = 64;

    struct Note
    {
        Note(NoteValue value, MotifInstance* motif, Node* node, ScoreTime startTime, ScoreTime duration)
            : value(value)
            , motif(motif)
            , node(node)
            , startTime(startTime)
            , duration(duration)
        {
        }

        NoteValue value;
        NoteRelativeValue relativeValue;
        MotifInstance* motif;
        Node* node;
        ScoreTime startTime;
        ScoreTime duration;
        ScoreFrame* scoreFrame;
    };
}
