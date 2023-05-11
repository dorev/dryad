#pragma once

#include "types.h"

namespace Dryad
{
    struct Edge
    {
        NodePtr source;
        NodePtr destination;
        Int8 modulation;
        UInt32 weight;
        bool onlyForExit;
    };
}
