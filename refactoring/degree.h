#pragma once

#include "model_definitions.h"

namespace dryad
{
namespace model
{

struct scale_t;

struct degree_t
{
    degree_t(int interval_from_root, scale_weak_ptr parent_scale)
        : interval_from_root(interval_from_root)
        , parent_scale(parent_scale)
    {}

    int interval_from_root;
    scale_weak_ptr parent_scale;
    degree_weak_ptr next;
    degree_weak_ptr prev;
};

} // namespace model
} // namespace dryad