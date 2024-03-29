#pragma once

#include "types.h"
#include "harmonytransition.h"
#include "harmonyframe.h"

namespace Dryad
{
    class Score;

    class IHarmonyStrategy
    {
    public:
        virtual Result ApplyTransition(Score& score, HarmonyTransition& transition) = 0;
    };

    class HarmonyStrategy : IHarmonyStrategy
    {
    public:
        virtual Result ApplyTransition(Score& score, HarmonyTransition& transition) override;

    private:
        Result FirstFrame(Score& score, HarmonyTransition& transition);
        Result ChangeScale(Score& score, HarmonyTransition& transition);
        Result ChangeGraph(Score& score, HarmonyTransition& transition);
    };
}
