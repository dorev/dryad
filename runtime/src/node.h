#pragma once

#include "types.h"
#include "time.h"
#include "chord.h"
#include "edge.h"
#include "graph.h"

namespace Dryad
{
    struct Node
    {
        GraphPtr graph;
        Vector<EdgePtr> edgesIn;
        Vector<EdgePtr> edgesOut;
        Chord Chord;
        Duration duration;
        bool overrideGraphNoteRange;
        NoteRange noteRange;
        bool graphEntry;
        bool graphExit;
        void* userData;
    };

    using NodePtr = SharedPtr<Node>;
}
