#pragma once

#include "types.h"
#include "random.h"

namespace Dryad
{
    class Node;

    class Edge
    {
    public:
        Node* source;
        Node* destination;
        UInt32 weight;
        Int8 modulation;
        bool onlyForExit;

        static Edge* SelectWeightedRandom(const Vector<Edge*>& edges)
        {
            Vector<UInt32> cumulativeWeights(edges.Size());
            UInt32 totalWeight = 0;
            for (UInt32 i = 0; i < edges.Size(); ++i)
            {
                Edge* edge = edges[i];
                if(edge != nullptr)
                {
                    totalWeight += edge->weight;
                    cumulativeWeights[i] = totalWeight;
                }
            }

            UInt32 random = Random(1, totalWeight);
            UInt32 index = static_cast<UInt32>(std::lower_bound(cumulativeWeights.begin(), cumulativeWeights.end(), random) - cumulativeWeights.begin());
            return edges[index];
        }
    };
}
