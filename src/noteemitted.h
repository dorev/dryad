#pragma once

#include "types.h"

namespace Dryad
{
    class NoteEmitted
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
