#pragma once

#include "definitions.h"
#include "monitoring.h"

namespace dryad
{

struct degree_t;
struct harmony_graph_t;

struct harmony_node_t : monitor_count<harmony_node_t>
{
    harmony_node_t()
        : alteration(0)
        , inversion(0)
        , max_visit(1)
        , modulation(0)
        , visit_count(0)
        , is_entry(false)
        , is_exit(false)
        , name("")
    {}
    
    int alteration;
    int inversion;
    int max_visit;
    int modulation;
    int visit_count;
    bool is_entry;
    bool is_exit;
    std::string name;
    std::vector<harmony_node_weak_ptr> edges;
    harmony_graph_weak_ptr parent_harmony_graph;
    degree_weak_ptr degree;
};

} // namespace dryad