#pragma once

#include "dryad/types.h"
#include "dryad/model/motifvariation.h"

namespace Dryad::Model
{

class Motif
{
    Vector<MotifVariation> _variations;

    void GenerateVariation(const MotifConfig& config);

    /*
    void spend_melodic_energy(motif_variation_ptr motif, motif_config_ptr motif_config);
    void spend_rhythmic_energy(motif_variation_ptr motif, motif_config_ptr motif_config);
    void generate_motif(motif_variation_ptr motif, motif_config_ptr motif_config);
    void generate_motif(motif_ptr motif, motif_config_ptr motif_config);
    */
};

} // namespace Dryad::Model