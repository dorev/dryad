#pragma once

#include "types.h"
#include "time.h"

namespace Dryad
{
    struct NoteEmitted
    {
        NoteValue value;
        Time timeOn;
        Time timeOff;
        IdType motifId;
        void* motifUserData;
        void* graphUserData;
        void* nodeUserData;
    };
}
