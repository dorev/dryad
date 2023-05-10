#pragma once

#include "types.h"
#include "identification.h"
#include "chord.h"

namespace Dryad
{
    class Graph : public Named
    {
        Vector<NodePtr> nodes;
        Vector<EdgePtr> entryEdges;
        Vector<EdgePtr> exitEdges;
        TimeSignature timeSignature;
        NoteRange noteRange;
    };
}
