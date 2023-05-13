#pragma once

#include "types.h"

class NodePtr;

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

    using EdgePtr = SharedPtr<Edge>;
}
