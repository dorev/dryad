#pragma once

#include "types.h"
#include "identification.h"
#include "chord.h"

namespace Dryad
{
    class Node : public Named
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
