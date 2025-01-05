#pragma once

#include "types.h"

namespace dryad
{
    inline dryad_time nearest_beat_before(dryad_time beat_value, dryad_time score_time)
    {
        dryad_time remainder = score_time % beat_value;
        if (remainder == 0)
        {
            return score_time - beat_value;
        }
        else
        {
            return score_time - remainder;
        }
    }

    inline dryad_time nearest_beat_after(dryad_time beat_value, dryad_time score_time)
    {
        dryad_time remainder = score_time % beat_value;
        if (remainder == 0)
        {
            return score_time + beat_value;
        }
        else
        {
            return score_time + (beat_value - remainder);
        }
    }
}
