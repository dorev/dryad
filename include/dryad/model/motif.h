#pragma once

#include "dryad/model/definitions.h"

namespace dryad
{

struct motif_variation_t;

struct motif_t
{
    std::vector<motif_variation_ptr> variations;
    score_weak_ptr parent_score;
};

} // namespace dryad