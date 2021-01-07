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
    node->degree = degree;
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
            DEBUG_BREAK("harmony_graph has been deallocated before its nodes");
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
        DEBUG_BREAK("harmony_node is left more than it was visited");
    }

    node->visit_count--;

    if (progression.empty())
    {
        DEBUG_BREAK("a progression cannot be empty if a node has been visited");
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

const std::vector<int>& get_chord_interval(degree_ptr degree)
{
    if (degree->chord_intervals.size() > 0)
    {
        return degree->chord_intervals;
    }

    degree_ptr third = next(next(degree));
    degree_ptr fifth = next(next(third));
    degree_ptr seventh = next(next(fifth));

    int root = degree->interval_from_root;

    int third_interval = third->interval_from_root < root
        ? third->interval_from_root + 12 - root
        : third->interval_from_root - root;

    if (third_interval != 3 && third_interval != 4)
    {
        DEBUG_BREAK("unsupported third chord interval");
    }

    int fifth_interval = fifth->interval_from_root < root
        ? fifth->interval_from_root + 12 - root
        : fifth->interval_from_root - root;

    if (fifth_interval != 7)
    {
        if (fifth_interval == 6 && third_interval == 3)
        {
            degree->chord_intervals = _dim_;
            return degree->chord_intervals;
        }
        else if (fifth_interval == 8 && third_interval == 4)
        {
            degree->chord_intervals = _aug_;
            return degree->chord_intervals;
        }

        DEBUG_BREAK("unsupported fifth chord interval");
    }

    int seventh_interval = seventh->interval_from_root < root
        ? seventh->interval_from_root + 12 - root
        : seventh->interval_from_root - root;

    if (seventh_interval != 10 && third_interval != 11)
    {
        DEBUG_BREAK("unsupported seventh chord interval");
    }

    if (third_interval == 3)
    {
        if (seventh_interval == 10)
        {
            degree->chord_intervals = _m7_;
        }
        else
        {
            degree->chord_intervals = _mM7_;
        }

        return degree->chord_intervals;
    }
    else
    {
        if (seventh_interval == 10)
        {
            degree->chord_intervals = _M7_;
        }
        else
        {
            degree->chord_intervals = _MM7_;
        }

        return degree->chord_intervals;
    }
}

harmony_graph_ptr create_major_graph()
{
    harmony_graph_ptr graph = std::make_shared<harmony_graph_t>();
    graph->scale = create_major_scale();

    for (int i = 0; i < 7; ++i)
    {
        harmony_node_ptr node = std::make_shared<harmony_node_t>();
        node->degree = graph->scale->degrees[i];
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
        DEBUG_BREAK("no notes to spend melodic energy on");
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
            DEBUG_BREAK("reached unsupported rhythmic fraction during rhythmic energy spending");
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
        DEBUG_BREAK("motif duration should be a factor of quarter notes");
    }
    if (motif_config->duration > 2 * _whole_)
    {
        DEBUG_BREAK("request motif duration is too long");
    }

    spend_rhythmic_energy(motif, motif_config);
    spend_melodic_energy(motif, motif_config);
}

void generate_motif(motif_ptr motif, motif_config_ptr motif_config)
{
    if (motif_config->duration % _quarter_)
    {
        DEBUG_BREAK("motif duration should be a factor of quarter notes");
    }
    if (motif_config->duration > 2 * _whole_)
    {
        DEBUG_BREAK("request motif duration is too long");
    }

    if (motif->variations.size() == 0)
    {
        motif->variations.emplace_back(std::make_shared<motif_variation_t>());
        generate_motif(motif->variations[0], motif_config);
        return;
    }

    DEBUG_BREAK("generate_motif should not be called on a motif already containing variations");
}

void apply_progression(phrase_ptr phrase, std::vector<harmony_node_ptr>& progression, fitting_mode_e fitting_mode)
{
    std::vector<measure_ptr>& measures = phrase->measures;
    int progression_size = static_cast<int>(progression.size());
    int phrase_size = static_cast<int>(measures.size());

    if (!is_power_of_2(phrase_size))
    {
        DEBUG_BREAK("A phrase should be a power of 2");
    }

    // Perfect fit! Easy dispatch
    if (progression_size == phrase_size)
    {
        for (int i = 0; i < phrase_size; ++i)
        {
            measures[i] = phrase->progression[i];
        }
        return;
    }

    std::vector<int> degrees_distribution(phrase_size, 0);

    // Values that cannot be initialized in the switch case
    int chords_to_fit = progression_size;
    int offset = phrase_size;
    int measure = 0;

    switch (fitting_mode)
    {
    case fitting_mode_e::power_of_2_left:
    case fitting_mode_e::power_of_2_right:

        for (int n = 0; n < chords_to_fit; ++n)
        {
            offset = phrase_size;
            measure = 0;

            for(int bit = 0; bit < std::log2(phrase_size); ++bit)
            {
                offset >>= 1;

                bool bit_of_n_is_off = !((1 << bit) & n);

                if (bit_of_n_is_off)
                {
                    measure += offset;
                }
            }

            if (progression_size < phrase_size ||
                fitting_mode == fitting_mode_e::power_of_2_left)
            {
                ++degrees_distribution[(phrase_size - 1) - measure];
            }
            else if (fitting_mode == fitting_mode_e::power_of_2_right)
            {
                ++degrees_distribution[measure];
            }
            else
            {
                DEBUG_BREAK("We should not reach this point");
            }
        }

        break;

    case fitting_mode_e::compact_left:
    case fitting_mode_e::compact_right:

        while (chords_to_fit != 0)
        {
            bool break_while = false;

            for (measure = phrase_size; measure > (phrase_size - offset); --measure)
            {
                if (progression_size < phrase_size ||
                    fitting_mode == fitting_mode_e::compact_left)
                {
                    ++degrees_distribution[phrase_size - measure];
                }
                else if (fitting_mode == fitting_mode_e::compact_right)
                {
                    ++degrees_distribution[measure - 1];
                }
                else
                {
                    DEBUG_BREAK("We should not reach this point");
                }

                if (--chords_to_fit == 0)
                {
                    break_while = true;
                    break;
                }
            }
            
            if (break_while)
            {
                break;
            }

            offset >>= 1;

            if (offset == 0)
            {
                offset = phrase_size;
            }
        }

    default:
        break;
    }

    int progression_index = 0;

    for(int i = 0; i < phrase_size; ++i)
    {
        // For the number of chords in that measure
        while (degrees_distribution[i]--)
        {
            measures[i] = phrase->progression[progression_index++];
        }
    }
}

void apply_motif(phrase_ptr phrase, motif_variation_ptr motif_variation, voice_ptr voice)
{
    // Append the motif in loop until the phrase is fully filled
    for (;;)
    {
        // For each note of the motif
        for (note_ptr note : motif_variation->notes)
        {
            // Find the last measure with room for the target voice
            for (measure_ptr measure : phrase->measures)
            {
                int voice_duration = get_total_voice_duration(measure, voice);
                
                if (voice_duration < measure->duration)
                {
                    note_ptr cloned_note = std::make_shared<note_t>(*note);
                    cloned_note->voice = voice;
                    append_note(measure, cloned_note);

                    // Append next note
                    break;
                }
                else if (next(measure) == nullptr)
                {
                    // Whole phrase has been covered
                    return;
                }
            }
        }
    }
}

void append_note(measure_ptr measure, note_ptr note)
{
    int voice_duration = get_total_voice_duration(measure, note->voice);

    if (voice_duration >= measure->duration)
    {
        // Try to append on next measure
        if (measure_ptr next_measure = next(measure))
        {
            append_note(next_measure, note);
        }
        else
        {
            DEBUG_BREAK("attempted to append a note in a measure already full, with no following measure to fallback");
        }

        return;
    }

    position_ptr note_position = get_position_at_time(measure, voice_duration);

    if (note_position == nullptr)
    {
        note_position = insert_position_at_time(measure, voice_duration);
    }

    note_position->notes.emplace_back(note);
}

position_ptr get_position_at_time(measure_ptr measure, int time)
{
    for (position_ptr position : measure->positions)
    {
        if (position->measure_time == time)
        {
            return position;
        }
    }

    return nullptr;
}

position_ptr insert_position_at_time(measure_ptr measure, int time)
{
    position_ptr position_before = nullptr;
    position_ptr position_after = nullptr;

    for (position_ptr position : measure->positions)
    {
        int position_time = position->measure_time;

        if (position_time == time)
        {
            return position;
        }
        else if (position_time > time)
        {
            position_after = position;
            break;
        }

        position_before = position;
    }

    if (position_before == nullptr)
    {
        position_ptr new_position = measure->positions.emplace_back(std::make_shared<position_t>());
        new_position->measure_time = time;
        new_position->parent_measure = measure;
        return;
    }

    // TODO: insert position in vector in correct place and relink next/prev
}

int get_total_voice_duration(measure_ptr measure, voice_ptr voice)
{
    int total_duration = 0;

    if (measure->positions.size() == 0)
    {
        return 0;
    }

    bool voice_exists_in_measure = false;

    for (note_ptr note : measure->positions[0]->notes)
    {
        if (note->voice == voice)
        {
            voice_exists_in_measure = true;
            break;
        }
    }

    if (!voice_exists_in_measure)
    {
        return 0;
    }

    for (position_ptr position : measure->positions)
    {
        for (note_ptr note : position->notes)
        {
            if (note->voice == voice)
            {
                total_duration += note->duration;

                // It is assumed that there can only be
                // one note of a voice in a position
                break;
            }
        }
    }

    return total_duration;
}

} // namespace dryad