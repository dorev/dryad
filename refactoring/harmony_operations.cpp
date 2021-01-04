#include "harmony_operations.h"
#include "model_operations.h"
#include "degree.h"
#include "scale.h"
#include "harmony_graph.h"
#include "harmony_node.h"

namespace dryad
{
namespace harmony
{

using namespace model;

scale_ptr create_major_scale()
{
    scale_ptr major_scale = std::make_shared<scale_t>();

    add_degrees(major_scale, 
    {
        std::make_shared<degree_t>(0, major_scale),
        std::make_shared<degree_t>(2, major_scale),
        std::make_shared<degree_t>(4, major_scale),
        std::make_shared<degree_t>(5, major_scale),
        std::make_shared<degree_t>(7, major_scale),
        std::make_shared<degree_t>(9, major_scale),
        std::make_shared<degree_t>(11, major_scale),
    });

    for (int i = 0; i < 7; ++i)
    {
        major_scale->degrees[i]->next = major_scale->degrees[(i + 1) % 7];
        major_scale->degrees[(i + 1) % 7]->prev = major_scale->degrees[i];
    }

    return major_scale;
}

harmony_graph_ptr create_major_graph()
{
    harmony_graph_ptr graph = std::make_shared<harmony_graph_t>();
    graph->scale = create_major_scale();

    for (int i = 0; i < 7; ++i)
    {
        harmony_node_ptr node = std::make_shared<harmony_node_t>();
        node->associated_degree = graph->scale->degrees[i];
        node->parent_harmony_graph = graph;
        add_node(graph, node);
    }

    harmony_node_ptr I = graph->nodes[0];
    harmony_node_ptr ii = graph->nodes[1];
    harmony_node_ptr iii = graph->nodes[2];
    harmony_node_ptr IV = graph->nodes[3];
    harmony_node_ptr V = graph->nodes[4];
    harmony_node_ptr vi = graph->nodes[5];
    harmony_node_ptr viid = graph->nodes[6];

    mark_as_entry({I, iii, IV, V, vi});
    mark_as_exit({I, IV, V, vi, viid});

    add_edges(I,    {ii, iii, IV, V, vi, viid});
    add_edges(ii,   {V, viid});
    add_edges(iii,  {vi});
    add_edges(IV,   {V, viid});
    add_edges(V,    {I});
    add_edges(vi,   {ii, V});
    add_edges(viid, {I, iii});

    set_max_visit(I, 2);
    set_max_visit(IV, 2);
    set_max_visit(V, 2);
    set_max_visit(vi, 2);

    return graph;
}

} // namespace harmony
} // namespace dryad