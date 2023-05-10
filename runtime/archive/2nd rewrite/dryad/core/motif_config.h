#pragma once

#include "definitions.h"

namespace dryad
{

struct motif_config_t
{
    motif_config_t(int duration = 0, int melodic_energy = 0, int rhythmic_energy = 0)
        : duration(duration)
        , melodic_energy(melodic_energy)
        , min_melodic_energy(1)
        , max_melodic_energy(8)
        , rhythmic_energy(rhythmic_energy)
    {}

    int duration;
    int melodic_energy;
    int min_melodic_energy;
    int max_melodic_energy;
    int rhythmic_energy;
};

} // namespace dryad