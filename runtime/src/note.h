#pragma once

#include "types.h"

namespace Dryad
{
    class Note
    {
        NoteValue value;
        MotifPtr motif;
        NodePtr node;
        Position position;
        Duration duration;
        bool committed;
    };
}
