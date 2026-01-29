#pragma once

#include "serialize.h"

namespace Dryad
{

    struct SerializedGraph
    {
        Vector<SerializedProgression> progressions;
        int activeProgressionIndex = -1;
        Vector<SerializedMotif> motifs;
    };

    Error writeGraphToFile(const SerializedGraph& graph, const String& path);
    Error readGraphFromFile(const String& path, SerializedGraph& graph);

} // namespace Dryad
