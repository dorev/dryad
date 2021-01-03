#pragma once

#include "model_definitions.h"

namespace dryad
{
namespace model
{

// Score
score_ptr create_score();

// Harmony node
void set_degree(harmony_node_ptr node, degree_ptr degree);
void set_alteration(harmony_node_ptr node, int alteration);
void set_inversion(harmony_node_ptr node, int inversion);
void set_modulation(harmony_node_ptr node, int modulation);
void mark_as_entry(harmony_node_ptr node);
void mark_as_exit(harmony_node_ptr node);
void set_max_visit(harmony_node_ptr node, int max_visit);
void add_edge(harmony_node_ptr node, harmony_node_ptr other_node);
void add_edges(harmony_node_ptr node, std::initializer_list<harmony_node_ptr> other_nodes);
bool is_visitable(harmony_node_ptr node);
void visit(harmony_node_ptr node, std::vector<harmony_node_ptr>& progression);
void leave(harmony_node_ptr node, std::vector<harmony_node_ptr>& progression);

// Harmony graph
void add_node(harmony_graph_ptr graph, harmony_node_ptr node);
void add_nodes(harmony_graph_ptr graph, std::initializer_list<harmony_node_ptr> nodes);
void generate_progressions(harmony_graph_ptr graph);

// Score construction
void apply_harmony(note_ptr note, harmony_node_ptr node);
void apply_scale(score_ptr score, scale_ptr scale);

// Score rendering
void render_musicxml(score_t* score);

} // namespace model
} // namespace dryad