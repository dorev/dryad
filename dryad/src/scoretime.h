#pragma once

#include "types.h"

namespace Dryad
{

    inline Time nearestBeatBefore(Time beatValue, Time scoreTime)
    {
        Time remainder = scoreTime % beatValue;

        if (remainder == 0)
        {
            return scoreTime - beatValue;
        }
        else
        {
            return scoreTime - remainder;
        }
    }

    inline Time nearestBeatAfter(Time beatValue, Time scoreTime)
    {
        Time remainder = scoreTime % beatValue;

        if (remainder == 0)
        {
            return scoreTime + beatValue;
        }
        else
        {
            return scoreTime + (beatValue - remainder);
        }
    }

} // namespace Dryad
