#include "score.h"
#include "harmoniccontext.h"

namespace Dryad
{
    void Score::Reset(Time startTime)
    {
        info.startTime = startTime;
        info.committedDuration = MusicTimeZero;
        info.generatedDuration = MusicTimeZero;
        info.currentHarmonicContext = nullptr;
    }
}
