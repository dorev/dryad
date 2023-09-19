#pragma once

#include "types.h"

namespace Dryad
{
    class Node;
    class MotifInstance;

    struct Note
    {
        Note(NoteValue value, MotifInstance* motif, Node* node, ScoreTime position, ScoreTime duration)
            : value(value)
            , motif(motif)
            , node(node)
            , position(position)
            , duration(duration)
        {
        }

        NoteValue value;
        NoteRelativeValue relativeValue;
        MotifInstance* motif;
        Node* node;
        ScoreTime position;
        ScoreTime duration;
    };
}
