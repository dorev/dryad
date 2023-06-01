#pragma once

#include "types.h"

namespace Dryad
{
    class Node;
    class Motif;

    struct Note
    {
        NoteValue value;
        Motif* motif;
        Node* node;
        ScoreTime position;
        ScoreTime duration;
        bool committed;
    };
}
