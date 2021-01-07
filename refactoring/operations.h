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
const std::vector<int>& get_chord_interval(degree_ptr degree);


// Motif
void spend_melodic_energy(motif_variation_ptr motif, motif_config_ptr motif_config);
void spend_rhythmic_energy(motif_variation_ptr motif, motif_config_ptr motif_config);
void generate_motif(motif_variation_ptr motif, motif_config_ptr motif_config);
void generate_motif(motif_ptr motif, motif_config_ptr motif_config);

// Phrase
void apply_progression(phrase_ptr phrase, std::vector<harmony_node_ptr>& progression, fitting_mode_e fitting_mode);
void apply_motif(phrase_ptr phrase, motif_variation_ptr motif_variation, voice_ptr voice);
void append_note(measure_ptr measure, note_ptr note);

// Measure
int get_total_voice_duration(measure_ptr measure, voice_ptr voice);
position_ptr get_position_at_time(measure_ptr measure, int time);
position_ptr insert_position_at_time(measure_ptr measure, int time);


template <class T>
typename std::enable_if<std::is_same<std::weak_ptr<T>, decltype(T::next)>::value, std::shared_ptr<T>>::type
next(std::shared_ptr<T> item)
{
    return item->next.lock();
}

template <class T>
typename std::enable_if<std::is_same<std::weak_ptr<T>, decltype(T::prev)>::value, std::shared_ptr<T>>::type
prev(std::shared_ptr<T> item)
{
    return item->prev.lock();
}

// Score construction
void apply_harmony(note_ptr note, harmony_node_ptr node);
void apply_scale(score_ptr score, scale_ptr scale);

// Score rendering
void render_musicxml(score_t* score);

} // namespace dryad