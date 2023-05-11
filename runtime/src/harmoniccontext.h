#pragma once

#include "types.h"
#include "scale.h"

namespace Dryad
{
    class HarmonicContext
    {
        TimeSignature timeSignature;
        Position position;
        Tempo tempo;
        Scale scale;
        Map<MotifPtr, UInt32> motifLevels;
        NodePtr node;
        GraphPtr graph;
    };
}
