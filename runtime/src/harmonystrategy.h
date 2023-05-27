#pragma once

#include "types.h"
#include "harmonyframe.h"
#include "harmonytransition.h"

namespace Dryad
{
    class HarmonyStrategy
    {
    public:
        Result ApplyTransition(Deque<HarmonyFrame>& frames, const HarmonyTransition& transition)
        {
            return Result::NotYetImplemented;
        }
    };
}
