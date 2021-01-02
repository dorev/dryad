#pragma once

#include "includes.h"

namespace dryad
{
namespace model
{

struct degree;
struct harmony_graph;

struct harmony_node
{
    // Members
    int id;
    int inversion;
    int alteration;
    int modulation;
    std::vector<int> edges;

    // References
    std::shared_ptr<harmony_graph> parent_harmony_graph;
    std::shared_ptr<degree> associated_degree;
};

} // namespace model
} // namespace dryad