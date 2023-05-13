#include "score.h"
#include "harmoniccontext.h"

namespace Dryad
{
    void Score::Reset(Time startTime)
    {
        info.startTime = startTime;
        info.committedDuration = TimeZero;
        info.generatedDuration = TimeZero;
        info.currentHarmonicContext = nullptr;
    }
}
