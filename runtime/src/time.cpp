#include "time.h"

namespace Dryad
{
    ScoreTime NearestBeatBefore(ScoreTime beatValue, ScoreTime scoreTime)
    {
        ScoreTime remainder = scoreTime % beatValue;
        if (remainder == 0)
        {
            return scoreTime - beatValue;
        }
        else
        {
            return scoreTime - remainder;
        }
    }

    ScoreTime NearestBeatAfter(ScoreTime beatValue, ScoreTime scoreTime)
    {
        ScoreTime remainder = scoreTime % beatValue;
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
