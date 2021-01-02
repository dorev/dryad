#pragma once

#include "includes.h"
#include "model_definitions.h"

namespace dryad
{
namespace model
{

struct harmony_node;

struct harmony_graph
{
    // Members
    std::vector<harmony_node> nodes;
    std::vector<std::vector<degree_ptr>> progressions;
};

} // namespace model
} // namespace dryad