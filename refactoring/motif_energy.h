#pragma once

#include "definitions.h"

namespace dryad
{

struct motif_energy_t
{
    motif_energy_t(int duration, int melodic_energy, int rhythmic_energy)
        : duration(duration)
        , melodic_energy(melodic_energy)
        , min_melodic_energy(0)
        , max_melodic_energy(_fifth_)
        , rhythmic_energy(rhythmic_energy)
        , min_rhythmic_energy(0)
        , max_rhythmic_energy(_fifth_)
    {}

    int duration;
    int melodic_energy;
    int min_melodic_energy;
    int max_melodic_energy;
    int rhythmic_energy;
    int min_rhythmic_energy;
    int max_rhythmic_energy;
};

} // namespace dryad