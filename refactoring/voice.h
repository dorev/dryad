#pragma once

#include "model_definitions.h"

namespace dryad
{
namespace model
{

struct score_t;

struct voice_t
{
    std::string name;
    score_weak_ptr parent_score;
};

} // namespace model
} // namespace dryad