#pragma once

#include "definitions.h"

namespace dryad
{

struct score_t;

struct voice_t
{
    std::string name;
    score_weak_ptr parent_score;
};

} // namespace dryad