#include "modegraph.h"
#include "degreenode.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

mode_graph::mode_graph(scale_t scale) 
    : _scale(scale) 
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void mode_graph::generate_permutations(size_t max_prog_length)
{
    _progs.clear();

    // 0 == max length
    size_t effective_max_prog_length =
        (max_prog_length > 0 && max_prog_length < MAX_PROG_LENGTH)
        ? max_prog_length
        : MAX_PROG_LENGTH;

    // This vector will contain the progression_t created while exploring the graph
    // During graph exploration, nodes will be added and removed and copies of valid
    // progressions will be submitted to the _permutations vector.
    progression_t current_prog;
    current_prog.reserve(effective_max_prog_length);

    // Lambda called when visiting a node
    // Incrementing the node visit_count and push it on current progression_t
    auto visit = [&](degree_node* node) mutable
    {
        node->visit();
        current_prog.push_back(node);
    };

    // Lambda called when leaving a node
    // Decrement the node visit_count and pop it from current progression_t
    auto leave_node = [&](degree_node* node) mutable
    {
        node->leave();

        if (!current_prog.empty())
        {
            current_prog.pop_back();
        }
    };

    // Lambda called recursively to explore graph
    // Submits valid progressions to the _permutations vector
    std::function<void(degree_node*)> explore_node = [&](degree_node* node) mutable
    {
        visit(node);

        if (node->is_prog_exit()    &&
            current_prog.size() > 1 &&
            current_prog.size() <= effective_max_prog_length)
        {
            _progs.push_back(current_prog); 
        }
        
        for (degree_node* next_node : node->get_edges())
        {
            if (next_node->is_visitable())
            {
                explore_node(next_node);
            }
        }

        leave_node(node);
    };
    
    LOG("Generating permutations with parameters\n"
        "Max progression_t length : " << effective_max_prog_length);

    PROFILE(dryad_timer timer;)

    for (degree_node* node : _degrees)
    {
        if (node->is_prog_entry())
        {
            explore_node(node);
        }
    }

    PROFILE(LOG("Generated " << _progs.size() << " progressions in " << timer.stop().c_str());)
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void mode_graph::print_permutations()
{
    LOG(_progs.size() << " major permutations");

    int prog_counter = 0;

    for (progression_t& progression_t : _progs)
    {
        std::cout << std::setw(5) << ++prog_counter << " : ";
        for (degree_node* degree : progression_t)
        {
            std::cout << degree->get_name().c_str() << " ";
        }
        std::cout << "\n";
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const progression_t& mode_graph::random_prog(size_t min_length, size_t max_length)
{
    if (min_length != 0 && max_length != 0)
    {
        auto end_itr = std::partition(_progs.begin(), _progs.end(),[&](const progression_t& p)
        {
            return p.size() >= min_length && p.size() <= max_length;
        });

        int max = static_cast<int>(std::distance(_progs.begin(), end_itr) - 1);

        return _progs[random::range(0, max)];
    }
    else if (min_length == 0 && max_length == 0)
    {
        return _progs[random::range(unsigned int(0), _progs.size() - 1)];
    }

    CRASH("min_length and max_length should both be set or not");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}