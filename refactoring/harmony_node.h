#pragma once

#include "model_definitions.h"

namespace dryad
{
namespace model
{

struct degree_t;
struct harmony_graph_t;

struct harmony_node_t
{
    harmony_node_t()
        : alteration(0)
        , inversion(0)
        , max_visit_count(1)
        , modulation(0)
        , visit_count(0)
        , is_entry(0)
        , is_exit()
    {}
    
    int alteration;
    int inversion;
    int max_visit_count;
    int modulation;
    int visit_count;
    bool is_entry;
    bool is_exit;
    std::vector<harmony_node_ptr> edges;
    harmony_graph_weak_ptr parent_harmony_graph;
    degree_ptr associated_degree;
};

} // namespace model
} // namespace dryad