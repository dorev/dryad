#pragma once

#include "types.h"

namespace Dryad
{
    struct Note
    {
        NoteValue value;
        MotifPtr motif;
        NodePtr node;
        Position position;
        Duration duration;
        bool committed;
    };
}
