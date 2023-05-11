#pragma once

#include "types.h"
#include "chord.h"

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
}
