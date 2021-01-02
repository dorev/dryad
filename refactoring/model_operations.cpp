#include "harmony_graph.h"
#include "harmony_node.h"
#include "measure.h"
#include "model_operations.h"
#include "note.h"
#include "phrase.h"
#include "position.h"
#include "scale.h"
#include "score.h"

namespace dryad
{
namespace model
{

// Harmony graph construction
void add_node(harmony_graph_ptr graph, harmony_node_ptr node)
{
}

void set_degree(harmony_node_ptr node, degree_ptr degree)
{
}

void mark_as_entry(harmony_node_ptr node)
{
}

void mark_as_exit(harmony_node_ptr node)
{
}

void set_max_visit(harmony_node_ptr node, int max_visit)
{
}

void add_edge(harmony_node_ptr node, harmony_node_ptr other_node)
{
}

void add_edges(harmony_node_ptr node, std::initializer_list<harmony_node_ptr> other_nodes)
{
}

// Score construction
void apply_harmony(note_ptr note, harmony_node_ptr node)
{
}

void apply_scale(score_ptr score, scale_ptr scale)
{
}

// Score iteration
measure_ptr next(measure_ptr)
{
    return nullptr;
}

measure_ptr prev(measure_ptr)
{
    return nullptr;
}

note_ptr next(note_ptr)
{
    return nullptr;
}

note_ptr prev(note_ptr)
{
    return nullptr;
}

phrase_ptr next(phrase_ptr)
{
    return nullptr;
}

phrase_ptr prev(phrase_ptr)
{
    return nullptr;
}

position_ptr next(position_ptr)
{
    return nullptr;
}

position_ptr prev(position_ptr)
{
    return nullptr;
}

// Score rendering
void render_musicxml(score* score)
{
}



} // namespace model
} // namespace dryad