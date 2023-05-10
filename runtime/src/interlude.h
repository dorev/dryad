#pragma once

#include "types.h"
#include "identification.h"
#include "graph.h"
#include "motif.h"

namespace Dryad
{
    class Interlude : public Named
    {
        Graph graph;
        Motif melody;
    };
}
