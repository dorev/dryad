#include "degree.h"
#include "harmony_graph.h"
#include "harmony_node.h"
#include "measure.h"
#include "motif.h"
#include "motif_config.h"
#include "motif_variation.h"
#include "operations.h"
#include "note.h"
#include "phrase.h"
#include "position.h"
#include "utils.h"
#include "scale.h"
#include "score.h"

namespace dryad
{

// Creates an empty score with an empty harmony graph
score_ptr create_score()
{
    score_ptr score = std::make_shared<score_t>();
    score->graph = std::make_shared<harmony_graph_t>();
    score->graph->parent_score = score;

    return score;
}

void set_degree(harmony_node_ptr node, degree_ptr degree)
{
    node->associated_degree = degree;
}

void set_alteration(harmony_node_ptr node, int alteration)
{
    node->alteration = alteration;
}

void set_inversion(harmony_node_ptr node, int inversion)
{
    node->inversion = inversion;
}

void set_modulation(harmony_node_ptr node, int modulation)
{
    node->modulation = modulation;
}

void mark_as_entry(harmony_node_ptr node, bool value)
{
    node->is_entry = value;
}

void mark_as_entry(std::initializer_list<harmony_node_ptr> nodes, bool value)
{
    for (harmony_node_ptr node : nodes)
    {
        node->is_entry = value;
    }
}

void mark_as_exit(harmony_node_ptr node, bool value)
{
   node->is_exit = value;
}

void mark_as_exit(std::initializer_list<harmony_node_ptr> nodes, bool value)
{
    for (harmony_node_ptr node : nodes)
    {
        node->is_exit = value;
    }
}

void set_max_visit(harmony_node_ptr node, int max_visit)
{
    node->max_visit_count = max_visit;
}

void add_edge(harmony_node_ptr node, harmony_node_ptr other_node)
{
    for (const harmony_node_ptr& edge : node->edges)
    {
        if (edge == other_node)
        {
            return;
        }
    }

    node->edges.push_back(other_node);
}

void add_edges(harmony_node_ptr node, std::initializer_list<harmony_node_ptr> other_nodes)
{ 
    for (const harmony_node_ptr& other_node : other_nodes)
    {
        bool skip = false;

        for (const harmony_node_ptr& edge : node->edges)
        {
            if (edge == other_node)
            {
                skip = true;
                break;
            }
        }

        if (skip)
        {
            continue;
        }

        node->edges.push_back(other_node);
    }
}

bool is_visitable(harmony_node_ptr node)
{
    return node->visit_count < node->max_visit_count;
}

void visit(harmony_node_ptr node, std::vector<harmony_node_ptr>& progression)
{
    node->visit_count++;

    progression.push_back(node);

    if (node->is_exit &&
        progression.size() > 1)
    {
        if (harmony_graph_ptr graph = node->parent_harmony_graph.lock())
        {
            graph->progressions.push_back(progression);
        }
        else
        {
            throw "harmony_graph has been deallocated before its nodes";
        }
    }

    for (harmony_node_ptr edge : node->edges)
    {
        if (is_visitable(edge))
        {
            visit(edge, progression);
        }
    }

    leave(node, progression);
}

void leave(harmony_node_ptr node, std::vector<harmony_node_ptr>& progression)
{
    if (node->visit_count == 0)
    {
        throw "harmony_node is left more than it was visited";
    }

    node->visit_count--;

    if (progression.empty())
    {
        throw "a progression cannot be empty if a node has been visited";
    }

    progression.pop_back();
}

void generate_progressions(harmony_graph_ptr graph)
{
    std::vector<harmony_node_ptr> progression;

    for (harmony_node_ptr node : graph->nodes)
    {
        if (node->is_entry)
        {
            visit(node, progression);
        }
    }
}

// Harmony graph construction
void add_node(harmony_graph_ptr graph, harmony_node_ptr node)
{
    node->parent_harmony_graph = graph;
    graph->nodes.push_back(node);
}

void add_nodes(harmony_graph_ptr graph, std::initializer_list<harmony_node_ptr> nodes)
{
    for (harmony_node_ptr node : nodes)
    {
        node->parent_harmony_graph = graph;
        graph->nodes.push_back(node);
    }
}

void add_degree(scale_ptr scale, degree_ptr added_degree)
{
    for (const degree_ptr& existing_degree : scale->degrees)
    {
        if (existing_degree == added_degree)
        {
            return;
        }
    }

    scale->degrees.push_back(added_degree);
}

void add_degrees(scale_ptr scale, std::initializer_list<degree_ptr> added_degrees)
{
    for (degree_ptr degree : added_degrees)
    {
        bool skip = false;

        for (const degree_ptr& existing_degree : scale->degrees)
        {
            if (existing_degree == degree)
            {
                skip = true;
                break;
            }
        }

        if (skip)
        {
            continue;
        }

        scale->degrees.push_back(degree);
    }
}

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
    add_edges(vi,   {ii, IV});
    add_edges(viid, {I, iii});

    set_max_visit(I, 2);
    set_max_visit(IV, 2);
    set_max_visit(V, 2);
    set_max_visit(vi, 2);

    return graph;
}

// Score construction
void apply_harmony(note_ptr note, harmony_node_ptr node)
{
}

void apply_scale(score_ptr score, scale_ptr scale)
{
}

// Score rendering
void render_musicxml(score_t* score)
{
}

void spend_melodic_energy(motif_variation_ptr motif, motif_config_ptr motif_config)
{
    if (motif->notes.size() == 0)
    {
        throw "no notes to spend melodic energy on";
    }

    int energy_left = motif_config->melodic_energy;
    int max = motif_config->max_melodic_energy;
    int min = motif_config->min_melodic_energy;

    std::vector<int> energy_distribution(motif->notes.size(), 0);

    while(energy_left != 0)
    {
        int targeted_note = random::range(0ULL, energy_distribution.size() - 1);
        int energy_given = 0;

        if (energy_distribution[targeted_note] < max)
        {
            if (energy_left >= min)
            {
                energy_given = min;
            }
            else
            {
                energy_given = energy_left;
            }
        }
        else
        {
            // Find another note to give energy too
            std::vector<int> candidate_indices;

            for (int i = 0; i < (int)motif->notes.size(); ++i)
            {
                if (energy_distribution[i] < max)
                {
                    candidate_indices.emplace_back(i);
                }
            }
            
            if (candidate_indices.empty())
            {
                // all notes are maxed on energy, unable to spend anymore energy
                break;
            }

            targeted_note = random::in(candidate_indices);

            if (energy_left >= min)
            {
                energy_given = min;
            }
            else
            {
                energy_given = energy_left;
            }
        }

        if (energy_distribution[targeted_note] + energy_given > max)
        {
            energy_given = max - energy_distribution[targeted_note];
        }

        energy_distribution[targeted_note] += energy_given;
        energy_left -= energy_given;
    }

    for (int i = 0; i < int(energy_distribution.size()); ++i)
    {
        if (random::coin_flip())
        {
            // Give a negative direction to energy
            energy_distribution[i] -= 2 * energy_distribution[i];
        }

        motif->notes[i]->offset = energy_distribution[i];
    }
}

void spend_rhythmic_energy(motif_variation_ptr motif, motif_config_ptr motif_config)
{
    int duration = motif_config->duration;
    int energy_left = motif_config->rhythmic_energy;

    std::vector<int> notes_durations({ duration });
    std::vector<std::pair<int, int>> solutions;

    while(energy_left--)
    {
        int targeted_note = random::range(0, static_cast<int>(notes_durations.size() - 1));
        int note_duration = notes_durations[targeted_note];

        if (note_duration > _sixteenth_)
        {
            get_equivalent_duration_pairs(note_duration, solutions);
            const std::pair<int, int>& selected_solution = random::in(solutions);
            notes_durations[targeted_note] = selected_solution.first;
            notes_durations.insert(notes_durations.begin() + targeted_note, selected_solution.second);
        }
        else if (note_duration == _sixteenth_)
        {
            // Find another note to split
            std::vector<int> candidate_indices;

            for (int i = 0; i < (int)notes_durations.size(); ++i)
            {
                if (notes_durations[i] > _sixteenth_)
                {
                    candidate_indices.emplace_back(i);
                }
            }

            if (candidate_indices.empty())
            {
                // notes_durations vector contains only sixteenth, unable to spend anymore energy
                break;
            }

            int new_targeted_note = random::in(candidate_indices);
            get_equivalent_duration_pairs(notes_durations[new_targeted_note], solutions);
            const std::pair<int, int>& selected_solution = random::in(solutions);
            notes_durations[new_targeted_note] = selected_solution.first;
            notes_durations.insert(notes_durations.begin() + new_targeted_note, selected_solution.second);
        }
        else
        {
            throw "reached unsupported rhythmic fraction during rhythmic energy spending";
        }
    }

    for(int i = 0; i < (int)notes_durations.size(); ++i)
    {
        motif->notes.emplace_back(std::make_shared<note_t>())->duration = duration;
    }
}

void generate_motif(motif_variation_ptr motif, motif_config_ptr motif_config)
{
    if (motif_config->duration % _quarter_)
    {
        throw "motif duration should be a factor of quarter notes";
    }
    if (motif_config->duration > 2 * _whole_)
    {
        throw "request motif duration is too long";
    }

    spend_rhythmic_energy(motif, motif_config);
    spend_melodic_energy(motif, motif_config);
}

void generate_motif(motif_ptr motif, motif_config_ptr motif_config)
{
    if (motif_config->duration % _quarter_)
    {
        throw "motif duration should be a factor of quarter notes";
    }
    if (motif_config->duration > 2 * _whole_)
    {
        throw "request motif duration is too long";
    }

    if (motif->variations.size() == 0)
    {
        motif->variations.emplace_back(std::make_shared<motif_variation_t>());
        generate_motif(motif->variations[0], motif_config);
        return;
    }

    throw "generate_motif should not be called on a motif already containing variations";
}



} // namespace dryad