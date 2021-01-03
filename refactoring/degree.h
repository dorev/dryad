#pragma once

#include "model_definitions.h"

namespace dryad
{
namespace model
{

struct scale_t;

struct degree_t
{
    int interval_from_root;
    scale_weak_ptr parent_scale;
};

} // namespace model
} // namespace dryad