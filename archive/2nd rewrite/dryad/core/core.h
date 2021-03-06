#pragma once

#include "definitions.h"

#include "degree.h"
#include "harmony_graph.h"
#include "harmony_node.h"
#include "measure.h"
#include "motif.h"
#include "motif_config.h"
#include "motif_variation.h"
#include "core.h"
#include "note.h"
#include "phrase.h"
#include "position.h"
#include "utils.h"
#include "scale.h"
#include "scale_config.h"
#include "score.h"
#include "utils.h"
#include "voice.h"

namespace dryad
{

#define MAKE_FUNCTION(type)                                             \
template <class... types>                                               \
type##_ptr make_##type(types... args)                                   \
{                                                                       \
    return std::make_shared<type##_t>(std::forward<types>(args)...);    \
}

MAKE_FUNCTION(degree)
MAKE_FUNCTION(harmony_node)
MAKE_FUNCTION(harmony_graph)
MAKE_FUNCTION(measure)
MAKE_FUNCTION(motif)
MAKE_FUNCTION(motif_config)
MAKE_FUNCTION(motif_variation)
MAKE_FUNCTION(note)
MAKE_FUNCTION(phrase)
MAKE_FUNCTION(position)
MAKE_FUNCTION(scale)
MAKE_FUNCTION(scale_config)
MAKE_FUNCTION(score)
MAKE_FUNCTION(voice)

#undef MAKE_FUNCTION

// Harmony node
void mark_as_entry(std::initializer_list<harmony_node_ptr> nodes, bool value = true);
void mark_as_exit(std::initializer_list<harmony_node_ptr> nodes, bool value = true);
void add_edge(harmony_node_ptr node, harmony_node_ptr other_node);
void add_edges(harmony_node_ptr node, std::initializer_list<harmony_node_ptr> other_nodes);
bool is_visitable(harmony_node_ptr node);
void visit(harmony_node_ptr node, std::vector<harmony_node_weak_ptr>& progression);
void leave(harmony_node_ptr node, std::vector<harmony_node_weak_ptr>& progression);

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
void apply_progression(phrase_ptr phrase, const std::vector<harmony_node_weak_ptr>& progression, fitting_mode_e fitting_mode = fitting_mode_e::power_of_2_right);
void apply_motif(phrase_ptr phrase, motif_variation_ptr motif_variation, voice_ptr voice);
void append_phrase(score_ptr score, phrase_ptr phrase);
void append_measure(phrase_ptr phrase, measure_ptr measure);
void append_note(measure_ptr measure, note_ptr note);

// Measure
int get_total_voice_duration(measure_ptr measure, voice_ptr voice);
position_ptr get_position_at_time(measure_ptr measure, int time);
position_ptr insert_position_at_time(measure_ptr measure, int time);

// Score construction
score_ptr create_score();
void relink_score(score_ptr score);
void relink_phrase(phrase_ptr phrase);
void apply_scale(score_ptr score, scale_ptr scale, scale_config_ptr scale_config);
harmony_node_ptr resolve_harmony_node(position_ptr position);

// Score rendering
void render_musicxml(score_t* score);

} // namespace dryad