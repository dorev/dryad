#pragma once

#include "types.h"
#include "time.h"
#include "scale.h"
#include "node.h"
#include "graph.h"
#include "motif.h"

namespace Dryad
{
    class HarmonicContext
    {
    public:


        TimeSignature timeSignature;
        Position position;
        Tempo tempo;
        ScalePtr scale;
        Map<MotifPtr, UInt32> motifLevels;
        NodePtr node;
        GraphPtr graph;
    };

    using HarmonicContextPtr = SharedPtr<HarmonicContext>;
}
