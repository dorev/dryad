#pragma once

#include "types.h"


namespace Dryad
{
    class Node;

    class Edge
    {
    public:
        Node* source;
        Node* destination;
        Int8 modulation;
        UInt32 weight;
        bool onlyForExit;
    };
}
