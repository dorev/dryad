#pragma once

#include "types.h"

namespace Dryad
{
    class Node;
    class MotifInstance;

    struct Note
    {
        NoteValue value;
        MotifInstance* motif;
        Node* node;
        ScoreTime position;
        ScoreTime duration;
        bool committed;
    };
}
