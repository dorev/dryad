#pragma once

#include "types.h"
#include "time.h"
#include "node.h"
#include "motif.h"

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
