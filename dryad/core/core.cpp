#include "core.h"

namespace dryad
{

// Creates an empty score with an empty harmony graph
score_ptr create_score()
{
    score_ptr score = make_score();
    score->graph = make_harmony_graph();
    score->graph->parent_score = score;

    return score;
}

// Used to set a group of node as potential progression starting point
void mark_as_entry(std::initializer_list<harmony_node_ptr> nodes, bool value)
{
    for (harmony_node_ptr node : nodes)
    {
        node->is_entry = value;
    }
}

// Used to set a group of nodes as potential progression ending point
void mark_as_exit(std::initializer_list<harmony_node_ptr> nodes, bool value)
{
    for (harmony_node_ptr node : nodes)
    {
        node->is_exit = value;
    }
}

// Adds other_node as edge if it is not already there
void add_edge(harmony_node_ptr node, harmony_node_ptr other_node)
{
    for (const harmony_node_weak_ptr& edge : node->edges)
    {
        harmony_node_ptr locked_edge = edge.lock();

        if (locked_edge == nullptr)
        {
            DEBUG_BREAK("Unable to add edge to node, it is expired");
        }

        if (locked_edge == other_node)
        {
            return;
        }
    }

    node->edges.push_back(other_node);
}

// Adds multiple other_nodes as edges if they are not already there
void add_edges(harmony_node_ptr node, std::initializer_list<harmony_node_ptr> other_nodes)
{ 
    for (const harmony_node_ptr& other_node : other_nodes)
    {
        bool skip = false;

        for (const harmony_node_weak_ptr& edge : node->edges)
        {
            harmony_node_ptr locked_edge = edge.lock();

            if (locked_edge == nullptr)
            {
                DEBUG_BREAK("Unable to add edge to node, it is expired");
            }

            if (locked_edge == other_node)
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

// Used when generating progressions of a harmony_graph
// Checks if the node has not already been visit more often
// than allowed by the current graph setup
bool is_visitable(harmony_node_ptr node)
{
    return node->visit_count < node->max_visit;
}

// Recursive function used to generate possible progression
// of a harmony_graph
void visit(harmony_node_ptr node, std::vector<harmony_node_weak_ptr>& progression)
{
    node->visit_count++;

    // Adds this node to the progression currently being built
    progression.push_back(node);

    // If the node can conclude a progression, store this progression in its
    // current state in the list of the harmony_graph
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

    // For each harmony_node connected to this one
    for (harmony_node_weak_ptr edge : node->edges)
    {
        harmony_node_ptr locked_edge = edge.lock();

        if (locked_edge == nullptr)
        {
            DEBUG_BREAK("Node edge has expired, unable to explore graph");
        }

        // If the edge node has not been visited to often yet
        if (is_visitable(locked_edge))
        {
            // Recursive call
            visit(locked_edge, progression);
        }
    }

    // Every allowed progression from this point has been visited
    leave(node, progression);
}

// Takes a step back in the harmony_graph during progression generation
void leave(harmony_node_ptr node, std::vector<harmony_node_weak_ptr>& progression)
{
    if (node->visit_count == 0)
    {
        DEBUG_BREAK("harmony_node is left more than it was visited");
    }

    // The harmony_node is "un-visited" because it can still be encountered
    // on permutations originating from earlier nodes in the harmony_graph
    node->visit_count--;

    if (progression.empty())
    {
        DEBUG_BREAK("a progression cannot be empty if a node has been visited");
    }

    progression.pop_back();
}

// Triggers the exploration of the harmony_graph to generate all
// possible progressions bases on the contained harmony_nodes parameters
void generate_progressions(harmony_graph_ptr graph)
{
    std::vector<harmony_node_weak_ptr> progression;

    for (harmony_node_ptr node : graph->nodes)
    {
        if (node->is_entry)
        {
            visit(node, progression);
        }
    }
}

// Adds a harmony_node to a harmony_graph
void add_node(harmony_graph_ptr graph, harmony_node_ptr node)
{
    node->parent_harmony_graph = graph;
    graph->nodes.push_back(node);
}

// Adds multiple harmony_nodes to a harmony_graph
void add_nodes(harmony_graph_ptr graph, std::initializer_list<harmony_node_ptr> nodes)
{
    for (harmony_node_ptr node : nodes)
    {
        node->parent_harmony_graph = graph;
        graph->nodes.push_back(node);
    }
}

// Adds a degree to a scale if it does not exists yet in it
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

// Adds multiple degrees to a scale if they don't appear in it yet
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

// Creates a Major ionian scale
scale_ptr create_major_scale()
{
    scale_ptr major_scale = make_scale();

    major_scale->name = "Major Ionian scale";

    add_degrees(major_scale, 
    {
        make_degree(0, major_scale),
        make_degree(2, major_scale),
        make_degree(4, major_scale),
        make_degree(5, major_scale),
        make_degree(7, major_scale),
        make_degree(9, major_scale),
        make_degree(11, major_scale),
    });

    // Binds together the degrees so they can be used in cycles with
    // endless calls to next() and previous()
    for (int i = 0; i < 7; ++i)
    {
        major_scale->degrees[i]->next = major_scale->degrees[(i + 1) % 7];
        major_scale->degrees[(i + 1) % 7]->previous = major_scale->degrees[i];
    }

    return major_scale;
}

// Evaluates the type of chord of a degree based on the following
// degrees in the scale
const std::vector<int>& get_chord_interval(degree_ptr degree)
{
    // Do not execute this function if custom intervals are set
    if (degree->chord_intervals.size() > 0)
    {
        return degree->chord_intervals;
    }

    // Reach other degrees of the chord
    degree_ptr third = next(next(degree));
    degree_ptr fifth = next(next(third));
    degree_ptr seventh = next(next(fifth));

    int root = degree->interval_from_root;

    // Handle potential inverted order of inteval from root
    int third_interval = third->interval_from_root < root
        ? third->interval_from_root + 12 - root
        : third->interval_from_root - root;

    // Validate that third is either major or minor
    if (third_interval != 3 && third_interval != 4)
    {
        DEBUG_BREAK("unsupported third chord interval");
    }

    // Handle potential inverted order of inteval from root
    int fifth_interval = fifth->interval_from_root < root
        ? fifth->interval_from_root + 12 - root
        : fifth->interval_from_root - root;

    // Handle cases where the chord does not have a perfect fifth
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

    // Handle potential inverted order of inteval from root
    int seventh_interval = seventh->interval_from_root < root
        ? seventh->interval_from_root + 12 - root
        : seventh->interval_from_root - root;

    // Validate that seventh is either major or minor
    if (seventh_interval != 10 && third_interval != 11)
    {
        DEBUG_BREAK("unsupported seventh chord interval");
    }

    // Identify complete chord interval following the previous validations
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

// Create a harmony_graph based on the major ionian scale with a
// mostly agreeable configuration
harmony_graph_ptr create_major_graph()
{
    harmony_graph_ptr graph = make_harmony_graph();
    graph->scale = create_major_scale();

    for (int i = 0; i < 7; ++i)
    {
        harmony_node_ptr node = make_harmony_node();
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

    I->max_visit  = 2;
    IV->max_visit = 2;
    V->max_visit  = 2;
    vi->max_visit = 2;

    return graph;
}

// Creates a motif_variation based on the motif_config provided
// The energy is randomly distributed to the notes of the motif 
// and increases the pitch offset of the notes compared to their
// neighbours

// Should be called after spend_rhythmic_energy() otherwise there
// might be only one note in the motif
void spend_melodic_energy(motif_variation_ptr motif, motif_config_ptr motif_config)
{
    if (motif->notes.size() == 0)
    {
        DEBUG_BREAK("no notes to spend melodic energy on");
    }

    int energy_left = motif_config->melodic_energy;
    int max = motif_config->max_melodic_energy;
    int min = motif_config->min_melodic_energy;

    // Created a vector to store "how the energy will be distributed"
    std::vector<int> energy_distribution(motif->notes.size(), 0);

    while(energy_left != 0)
    {
        // Randomly determines what note will get the energy
        int targeted_note = static_cast<int>(random::range(0ULL, energy_distribution.size() - 1));
        int energy_given = 0;

        // If the note can still receive energy, according to
        // the motif_config
        if (energy_distribution[targeted_note] < max)
        {
            if (energy_left >= min)
            {
                energy_given = min == 0 ? 1 : min;
            }
            else
            {
                energy_given = energy_left;
            }
        }
        // Find another note to give energy too
        else
        {
            // Vector to store potential alternative energy receivers
            std::vector<int> candidate_indices;

            // Identify all notes still having room for more energy
            for (unsigned i = 0; i < motif->notes.size(); ++i)
            {
                if (energy_distribution[i] < max)
                {
                    candidate_indices.emplace_back(i);
                }
            }
            
            if (candidate_indices.empty())
            {
                // All notes are maxed on energy, unable to spend anymore energy
                break;
            }

            // Randomly select a note among the candidates
            targeted_note = random::in(candidate_indices);

            if (energy_left >= min)
            {
                energy_given = min == 0 ? 1 : min;
            }
            else
            {
                energy_given = energy_left;
            }
        }

        // Prevent note from getting too much energy
        if (energy_distribution[targeted_note] + energy_given > max)
        {
            energy_given = max - energy_distribution[targeted_note];
        }

        // Update energy value of the note
        energy_distribution[targeted_note] += energy_given;
        energy_left -= energy_given;
    }

    // Randomly decide for each note of the energy makes the note
    // get lower or higher and then assigned the final value
    // to the motif
    for (unsigned i = 0; i < energy_distribution.size(); ++i)
    {
        if (random::coin_flip())
        {
            // Give a negative direction to energy
            energy_distribution[i] -= 2 * energy_distribution[i];
        }

        motif->notes[i]->offset = energy_distribution[i];
    }
}

// Breaks a motif duration into multiple smaller durations
void spend_rhythmic_energy(motif_variation_ptr motif, motif_config_ptr motif_config)
{
    int duration = motif_config->duration;
    int energy_left = motif_config->rhythmic_energy;

    std::vector<int> notes_durations({ duration });
    std::vector<std::pair<int, int>> solutions;

    while(energy_left--)
    {
        int targeted_note = random::range(0, (int)notes_durations.size() - 1);
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
                // notes_durations vector contains only sixteenth,
                // unable to spend anymore energy
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
        note_ptr new_note = make_note();
        new_note->duration = duration;
        new_note->motif = motif;
        motif->notes.emplace_back(new_note);
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
        DEBUG_BREAK("requested motif duration is too long");
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
        DEBUG_BREAK("requested motif duration is too long");
    }

    if (motif->variations.size() == 0)
    {
        motif->variations.emplace_back(make_motif_variation());
        generate_motif(motif->variations[0], motif_config);
        return;
    }

    DEBUG_BREAK("generate_motif should not be called on a motif already containing variations");
}

void apply_progression(phrase_ptr phrase, const std::vector<harmony_node_weak_ptr>& progression, fitting_mode_e fitting_mode)
{
    std::vector<measure_ptr>& measures = phrase->measures;
    int progression_size = (int)progression.size();
    int phrase_size = (int)measures.size();

    if (!is_power_of_2(phrase_size))
    {
        DEBUG_BREAK("A phrase should be a power of 2");
    }

    // Perfect fit! Easy dispatch
    if (progression_size == phrase_size)
    {
        for (int i = 0; i < phrase_size; ++i)
        {
            measures[i]->progression.emplace_back(progression[i]);
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

        break;

    default:
        break;
    }

    int progression_index = 0;

    for(int i = 0; i < phrase_size; ++i)
    {
        // For the number of chords in that measure
        while (degrees_distribution[i]--)
        {
            measures[i]->progression.emplace_back(progression[progression_index++]);
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
                    note_ptr cloned_note = clone(note);
                    cloned_note->voice = voice;
                    append_note(measure, cloned_note);

                    // Append next note
                    break;
                }
                else if (measure == last(phrase->measures))
                {
                    // Whole phrase has been covered
                    return;
                }
            }
        }
    }
}

void append_phrase(score_ptr score, phrase_ptr phrase)
{
    std::vector<phrase_ptr>& phrases = score->phrases;
    phrase->parent_score = score;

    if (!phrases.empty())
    {
        if (phrase_ptr last_phrase = last(phrases))
        {
            last_phrase->next = phrase;
            phrase->previous = last_phrase;
        }
    }

    phrases.emplace_back(phrase);
}

void append_measure(phrase_ptr phrase, measure_ptr measure)
{
    std::vector<measure_ptr>& measures = phrase->measures;
    measure->parent_phrase = phrase;

    if (measures.empty())
    {
        if (measure_ptr last_measure = last(measures))
        {
            last_measure->next = measure;
            measure->previous = last_measure;
        }
        else if (phrase_ptr previous_phrase = previous(phrase))
        {
            measure_ptr last_measure = last(previous_phrase->measures);
            last_measure->next = measure;
            measure->previous = last_measure;
        }
    }
    else
    {
        if (measure_ptr last_measure = last(measures))
        {
            last_measure->next = measure;
            measure->previous = last_measure;
        }
    }

    if (measure->positions.size() != 0)
    {
        if (measure_ptr last_measure = last(measures))
        {
            if (position_ptr last_position = last(last_measure->positions))
            {
                position_ptr position = first(measure->positions);
                position->previous = last_position;
                last_position->next = position;
            }
        }
    }

    measures.emplace_back(measure);
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

        return;
    }

    position_ptr note_position = get_position_at_time(measure, voice_duration);

    if (note_position == nullptr)
    {
        note_position = insert_position_at_time(measure, voice_duration);
    }

    if( (note->duration + note_position->measure_time) > measure->duration)
    {
        // Overflow note duration to next measure
        // TODO: liaisons?
        int duration_this_measure = measure->duration - note_position->measure_time;

        // Try to append on next measure
        if (measure_ptr next_measure = next(measure))
        {
            int duration_next_measure = note->duration - duration_this_measure;
            note_ptr next_measure_cloned_note = clone(note);
            next_measure_cloned_note->duration = duration_next_measure;
            append_note(next_measure, next_measure_cloned_note);
        }

        note->duration = duration_this_measure;
    }

    note->parent_position = note_position;
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
    std::vector<position_ptr>& positions = measure->positions;

    for (position_ptr position : positions)
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
        position_ptr new_position = make_position(measure, time);
        new_position->parent_measure = measure;

        if (time != 0)
        {
            // Add initial position with a rest
            position_ptr zero_position = make_position(measure);
            zero_position->parent_measure = measure;

            resolve_harmony_node(zero_position);
            new_position->previous = zero_position;
            zero_position->next = new_position;

            if (measure_ptr previous_measure = previous(measure))
            {
                if (position_ptr last_position = last(previous_measure->positions))
                {
                    zero_position->previous = last_position;
                }
            }

            positions.emplace_back(zero_position);
        }

        positions.emplace_back(new_position);
        resolve_harmony_node(new_position);
        return new_position;
    }

    for(auto it = positions.begin(); it != positions.end(); ++it)
    {
        if (*it == position_before)
        {
            position_ptr new_position = make_position();
            new_position->parent_measure = measure;

            new_position->measure_time = time;
            new_position->parent_measure = measure;
            positions.insert(it, new_position);

            position_before->next = new_position;
            new_position->previous = position_before;

            if (position_after != nullptr)
            {
                new_position->next = position_after;
                position_after->previous = new_position;
            }

            resolve_harmony_node(new_position);
            return new_position;
        }
    }

    DEBUG_BREAK("this section should never be reached");
}

int get_total_voice_duration(measure_ptr measure, voice_ptr voice)
{
    if (measure->positions.size() == 0)
    {
        return 0;
    }

    bool voice_exists_in_measure = false;

    // Inspect first position of measure to check if the targeted voice is
    // present in the measure
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

    int total_duration = 0;

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

void relink_phrase(phrase_ptr phrase)
{
    std::vector<measure_ptr>& measures = phrase->measures;
    int measure_count = (int)measures.size();

    for (int measure_index = 0; measure_index < measure_count; ++measure_index)
    {
        measure_ptr measure = measures[measure_index];
        measure->parent_phrase = phrase;

        if (measure_index == 0)
        {
            if (phrase_ptr previous_phrase = previous(phrase))
            {
                if (measure_ptr last_measure = last(previous_phrase->measures))
                {
                    measure->previous = last_measure;
                    last_measure->next = measure;
                }
            }
        }
        else
        {
            measure->previous = measures[measure_index - 1];
        }

        if (measure_index < (measure_count - 1))
        {
            measure->next = measures[measure_index + 1];
        }
        else
        {
            measure->next.reset();
        }

        std::vector<position_ptr>& positions = measure->positions;
        int position_count = (int)positions.size();

        for (int position_index = 0; position_index < position_count; ++position_index)
        {
            position_ptr position = positions[position_index];
            position->parent_measure = measure;

            if (position_index == 0)
            {
                if (measure_ptr previous_measure = previous(measure))
                {
                    if (position_ptr previous_position = last(previous_measure->positions))
                    {
                        position->previous = previous_position;
                        previous_position->next = position;
                    }
                }
            }
            else
            {
                position->previous = positions[position_index - 1];
            }

            if (position_index < (position_count - 1))
            {
                position->next = positions[position_index + 1];
            }
            else
            {
                position->next.reset();
            }
        }
    }
}

void relink_score(score_ptr score)
{
    std::vector<phrase_ptr>& phrases = score->phrases;
    int phrase_count = (int)phrases.size();

    for (int phrase_index = 0; phrase_index < phrase_count; ++phrase_index)
    {
        phrase_ptr phrase = phrases[phrase_index];
        phrase->parent_score = score;

        if (phrase_index != 0)
        {
            phrase->previous = phrases[phrase_index - 1];
        }

        if (phrase_index < (phrase_count - 1))
        {
            phrase->next = phrases[phrase_index + 1];
        }
        else
        {
            phrase->next.reset();
        }

        relink_phrase(phrase);
    }
}

void apply_scale(note_ptr note, scale_ptr scale, scale_config_ptr scale_config)
{
    position_ptr position = note->parent_position.lock();

    if (position == nullptr)
    {
        DEBUG_BREAK("note has no parent_position assigned");
    }

    harmony_node_ptr harmony_node = position->harmony_node;

    degree_ptr harmony_node_degree = harmony_node->degree.lock();

    if (harmony_node_degree == nullptr)
    {
        DEBUG_BREAK("No degree associated with node");
    }

    int midi_value = note->midi = scale_config->root +
        harmony_node_degree->interval_from_root +
        harmony_node->alteration +
        harmony_node->modulation +
        (12 * note->voice->octave);

    note->octave = midi_value / 12;

    int absolute_note = midi_value % 12;

    if (contains(absolute_note, _base_notes_))
    {
        note->name = _note_names_sharp_[absolute_note];
        note->accidental = accidental_e::none;
    }
    else
    {
        switch (scale_config->accidental)
        {
            default:
            case accidental_e::none:
            case accidental_e::sharp:
                note->accidental = accidental_e::sharp;
                note->name = _note_names_sharp_[absolute_note];
                note->step = _note_names_sharp_[absolute_note - 1];
                return;

            case accidental_e::flat:
                note->accidental = accidental_e::flat;
                note->name = _note_names_flat_[absolute_note];
                note->step = _note_names_flat_[absolute_note + 1];
                return;
        }
    }
}

// Score construction
void apply_scale(score_ptr score, scale_ptr scale, scale_config_ptr scale_config)
{
    relink_score(score);

    for (phrase_ptr phrase : score->phrases)
    {
        for (measure_ptr measure : phrase->measures)
        {
            for (position_ptr position : measure->positions)
            {
                for (note_ptr note : position->notes)
                {
                    apply_scale(note, scale, scale_config);
                }
            }
        }
    }
}

harmony_node_ptr find_latest_harmony_node(position_ptr position)
{
    measure_ptr measure = position->parent_measure.lock();

    if (measure == nullptr)
    {
        DEBUG_BREAK("a position is always expected to have a parent_measure");
    }

    for (;;)
    {
        measure = previous(measure);

        if (measure == nullptr)
        {
            return nullptr;
        }

        if(!measure->progression.empty())
        {
            return last(measure->progression);
        }
    }
}

harmony_node_ptr resolve_harmony_node(position_ptr position)
{
    measure_ptr measure = position->parent_measure.lock();

    if (measure == nullptr)
    {
        position->harmony_node = nullptr;
        DEBUG_BREAK("a position is always expected to have a parent_measure");
    }

    int progression_size = (int)measure->progression.size();

    if (progression_size == 0)
    {
        return position->harmony_node = find_latest_harmony_node(position);
    }
    else if (progression_size == 1)
    {
        return position->harmony_node = measure->progression[0];
    }

    int chord_duration = measure->duration / progression_size;
    int good_node_index = position->measure_time / chord_duration;

    return position->harmony_node = measure->progression[good_node_index];
}

// Score rendering
void render_musicxml(score_t* score)
{
}

} // namespace dryad