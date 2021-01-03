#pragma once

#include "model_definitions.h"

namespace dryad
{
namespace model
{

struct degree_t;
struct measure_t;
struct score_t;

struct phrase_t
{
    std::vector<measure_ptr> measures;
    std::vector<degree_ptr> associated_degrees;
    score_weak_ptr parent_score;
    phrase_weak_ptr next;
    phrase_weak_ptr prev;
};

} // namespace model
} // namespace dryad