#pragma once

#include "types.h"
#include "time.h"

namespace Dryad
{
    class Node;
    class Motif;

    struct Note
    {
        NoteValue value;
        Motif* motif;
        Node* node;
        Position position;
        Duration duration;
        bool committed;
    };
}
