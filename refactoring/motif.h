#pragma once

#include "model_definitions.h"

namespace dryad
{
namespace model
{

struct motif_variation_t;

struct motif_t
{
    motif_variation_ptr variations;
    score_weak_ptr parent_score;
};

} // namespace model
} // namespace dryad