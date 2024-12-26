#include "scoretime.h"

namespace dryad
{
    dryad_score_time nearest_beat_before(dryad_score_time beatValue, dryad_score_time scoreTime)
    {
        dryad_score_time remainder = scoreTime % beatValue;
        if (remainder == 0)
        {
            return scoreTime - beatValue;
        }
        else
        {
            return scoreTime - remainder;
        }
    }

    dryad_score_time nearest_beat_after(dryad_score_time beatValue, dryad_score_time scoreTime)
    {
        dryad_score_time remainder = scoreTime % beatValue;
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
