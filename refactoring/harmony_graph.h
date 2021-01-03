#pragma once

#include "model_definitions.h"

namespace dryad
{
namespace model
{

struct harmony_node_t;

struct harmony_graph_t
{
    std::vector<harmony_node_ptr> nodes;
    std::vector<std::vector<harmony_node_ptr>> progressions;
    score_weak_ptr parent_score;
};

} // namespace model
} // namespace dryad