#include "modegraph.h"
#include "degreenode.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void mode_graph::generate_permutations(size_t max_node_visit, size_t max_prog_length)
{
    // This vector will contain the progression created while exploring the graph
    // During graph exploration, nodes will be added and removed and copies of valid
    // progressions will be submitted to the _permutations vector.
    std::vector<degree_node*> current_prog;
    current_prog.reserve(max_prog_length);

    // Lambda called when visiting a node
    // Incrementing the node visit_count and push it on current progression
    auto visit = [&](degree_node* node) mutable
    {
        if (node->get_visit_count() < max_node_visit)
        {
            node->visit();
            current_prog.push_back(node);
        }
        else
        {
            CRASHLOG("A node should not be visited if max_visit limit is exceeded");
        }
    };

    // Lambda called when leaving a node
    // Decrement the node visit_count and pop it from current progression
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

        if (node->is_prog_exit()
            && current_prog.size() > 1
            && current_prog.size() <= max_prog_length)
        {
            _permutations.push_back(current_prog); 
        }
        
        for (degree_node* next_node : node->get_edges())
        {
            if (next_node->get_visit_count() < max_node_visit)
            {
                explore_node(next_node);
            }
        }

        leave_node(node);
    };
    
    LOG("Generating permutations with parameters\n"
        "Max node visit         : " << max_node_visit << "\n"
        "Max progression length : " << max_prog_length);

    timer t;

    for (degree_node* node : _degrees)
    {
        if (node->is_prog_entry())
        {
            explore_node(node);
        }
    }

    LOG("Permutations generation completed in " << t.stop().c_str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void mode_graph::print_permutations()
{
    LOG(_permutations.size() << " major permutations");

    int prog_counter = 0;

    for (std::vector<degree_node*>& progression : _permutations)
    {
        std::cout << std::setw(5) << ++prog_counter << " : ";
        for (degree_node* degree : progression)
        {
            std::cout << degree->get_name().c_str() << " ";
        }
        std::cout << "\n";
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}