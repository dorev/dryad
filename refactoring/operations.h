#pragma once

#include "definitions.h"

namespace dryad
{

// Score
score_ptr create_score();

// Harmony node
void set_degree(harmony_node_ptr node, degree_ptr degree);
void set_alteration(harmony_node_ptr node, int alteration);
void set_inversion(harmony_node_ptr node, int inversion);
void set_modulation(harmony_node_ptr node, int modulation);
void mark_as_entry(harmony_node_ptr node, bool value = true);
void mark_as_entry(std::initializer_list<harmony_node_ptr> nodes, bool value = true);
void mark_as_exit(harmony_node_ptr node, bool value = true);
void mark_as_exit(std::initializer_list<harmony_node_ptr> nodes, bool value = true);
void set_max_visit(harmony_node_ptr node, int max_visit);
void add_edge(harmony_node_ptr node, harmony_node_ptr other_node);
void add_edges(harmony_node_ptr node, std::initializer_list<harmony_node_ptr> other_nodes);
bool is_visitable(harmony_node_ptr node);
void visit(harmony_node_ptr node, std::vector<harmony_node_ptr>& progression);
void leave(harmony_node_ptr node, std::vector<harmony_node_ptr>& progression);

// Harmony graph
harmony_graph_ptr create_major_graph();
void add_node(harmony_graph_ptr graph, harmony_node_ptr node);
void add_nodes(harmony_graph_ptr graph, std::initializer_list<harmony_node_ptr> nodes);
void generate_progressions(harmony_graph_ptr graph);

// Scale
scale_ptr create_major_scale();
void add_degree(scale_ptr scale, degree_ptr degree);
void add_degrees(scale_ptr scale, std::initializer_list<degree_ptr> degrees);

// Score construction
void apply_harmony(note_ptr note, harmony_node_ptr node);
void apply_scale(score_ptr score, scale_ptr scale);

// Score rendering
void render_musicxml(score_t* score);

// Motif
void spend_melodic_energy(motif_variation_ptr motif, motif_energy_ptr motif_energy);
void spend_rhythmic_energy(motif_variation_ptr motif, motif_energy_ptr motif_energy);
void generate_motif(motif_variation_ptr motif, motif_energy_ptr motif_energy);
void generate_motif(motif_ptr motif, motif_energy_ptr motif_energy);


// Detect "next" member and returns its value
template <class, class = void> struct has_next : std::false_type {};
template <class T> struct has_next<T, std::void_t<decltype(T::next)>> : std::true_type {};

template <class T>
typename std::enable_if<has_next<T>::value, std::shared_ptr<T>>::type
next(std::shared_ptr<T> item)
{
    return item->next.lock();
}

// Detect "prev" member and returns its value
template <class, class = void> struct has_prev : std::false_type {};
template <class T> struct has_prev<T, std::void_t<decltype(T::prev)>> : std::true_type {};

template <class T>
typename std::enable_if<has_prev<T>::value, std::shared_ptr<T>>::type
prev(std::shared_ptr<T> item)
{
    return item->prev.lock();
}


} // namespace dryad