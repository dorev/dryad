#pragma once

#include "types.h"

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
