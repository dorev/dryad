#pragma once

#include "types.h"
#include "identification.h"

namespace Dryad
{
    class Edge : public Identifiable
    {
        NodePtr source;
        NodePtr destination;
        Int8 modulation;
        UInt32 weight;
        bool onlyForExit;
    };
}
