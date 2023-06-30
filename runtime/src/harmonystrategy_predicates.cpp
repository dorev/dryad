#pragma once

#include "types.h"
#include "harmonystrategy.h"
#include "scale.h"

namespace Dryad
{
    // 1- If one of these frames is the next scale dominant, select it as pivot
    // 2- Explore the graph to find if a node nearby is a dominant
    // 3- Repeat 1-2 looking for IV
    // 4- Repeat 1-2 looking for V/V and add a V frame as pivot
    // 5- Repeat 1-2 looking for a common chord and use it as pivot
    // 6- Select the latest frame possible, hoping that something else happens in the meantime

    const FrameSearchPredicate HarmonyStrategy::FrameSearchPredicates[] =
    {
        [](const HarmonyFrame& frame, const Node* node, const Scale* scale) -> bool
        {
            // Find if any of the exit frame is the dominant of the new scale
            if(frame.node != nullptr && frame.node->graphExit)
            {
                return Chord::AreSimilar(frame.node->chord, scale->DominantChord());
            }
            return false;
        },
        [](const HarmonyFrame& frame, const Node* node, const Scale* scale) -> bool
        {
            // Find if any of the exit frame is the IV of the new scale
            if(frame.node != nullptr && frame.node->graphExit && scale != nullptr)
            {
                return Chord::AreSimilar(frame.node->chord, scale->SubdominantChord());
            }
            return false;
        },
    };

    const UInt32 HarmonyStrategy::FrameSearchPredicatesCount = sizeof(FrameSearchPredicates) / sizeof(FrameSearchPredicate);
}
