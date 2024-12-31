#include "scoretime.h"

namespace dryad
{
    dryad_time nearest_beat_before(dryad_time beatValue, dryad_time scoreTime)
    {
        dryad_time remainder = scoreTime % beatValue;
        if (remainder == 0)
        {
            return scoreTime - beatValue;
        }
        else
        {
            return scoreTime - remainder;
        }
    }

    dryad_time nearest_beat_after(dryad_time beatValue, dryad_time scoreTime)
    {
        dryad_time remainder = scoreTime % beatValue;
        if (remainder == 0)
        {
            return scoreTime + beatValue;
        }
        else
        {
            return scoreTime + (beatValue - remainder);
        }
    }
}
