#pragma once

#include "types.h"

namespace Dryad
{
    class Graph;
    class Motif;

    class Interlude
    {
    public:
        Graph* graph;
        Motif* melody;
    };
}
