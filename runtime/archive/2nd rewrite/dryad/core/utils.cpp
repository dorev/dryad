#include "utils.h"

namespace dryad
{

bool random::coin_flip()
{
    static thread_local std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(0, 1);
    return distribution(generator) > 0;
}

void get_equivalent_duration_pairs(int duration, std::vector<std::pair<int, int>>& solutions)
{
    solutions.clear();

    // Inverted solutions duplicates are deliberatly preserved
    for (int i = 0; i < (int)allowed_rhythmic_fractions.size(); ++i)
    {
        for (int j = 0; j < (int)allowed_rhythmic_fractions.size(); ++j)
        {
            if (allowed_rhythmic_fractions[i] + allowed_rhythmic_fractions[j] == duration)
            {
                solutions.emplace_back(allowed_rhythmic_fractions[i], allowed_rhythmic_fractions[j]);
            }
        }
    }

    if (solutions.empty())
    {
        DEBUG_BREAK("unable to split requested duration");
    }
}


} // namespace dryad
