#pragma once

#include "definitions.h"
#include "monitoring.h"

namespace dryad
{

struct scale_t;

struct degree_t : monitor_count<degree_t>
{
    degree_t(int interval_from_root, scale_weak_ptr parent_scale_, std::initializer_list<int> chord_intervals = {})
        : interval_from_root(interval_from_root)
        , chord_intervals(chord_intervals)
        , parent_scale(parent_scale_)
    {}

    int interval_from_root;
    std::vector<int> chord_intervals;
    scale_weak_ptr parent_scale;
    degree_weak_ptr next;
    degree_weak_ptr previous;
};

} // namespace dryad