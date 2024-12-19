#pragma once

#include "types.h"
#include "containers.h"
#include "graph.h"

namespace Dryad
{
    class Score
    {

        // has frames
        // has progression

        Graph graph;
    };

    class Progression : public Node
    {
        DRYAD_CLASS_ID(Progression);

    };

    class Chord : public Node
    {
        DRYAD_CLASS_ID(Chord);

    };

    class ScoreFrame : public Node
    {
        DRYAD_CLASS_ID(ScoreFrame);

    };

    class MotifInstance : public Node
    {
        DRYAD_CLASS_ID(MotifInstance);
    };

}
