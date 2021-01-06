#pragma once

#include "definitions.h"

namespace dryad
{

struct harmony_node_t;

struct harmony_graph_t
{
    scale_ptr scale;
    score_weak_ptr parent_score;
    std::vector<harmony_node_ptr> nodes;
    std::vector<std::vector<harmony_node_ptr>> progressions;
};

} // namespace dryad