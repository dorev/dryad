#include "modegraph.h"
#include "degreenode.h"

namespace dryad
{

void mode_graph::generate_permutations()
{
    std::deque<const degree_node*> visited;
    std::vector<const degree_node*> current_prog;

    auto visit = [&](const degree_node* node) mutable
    {
        if (std::find(visited.begin(), visited.end(), node) == visited.end())
        {
            visited.push_back(node);
            current_prog.push_back(node);
        }
    };

    auto unvisit_last_node = [&]() mutable
    {
        if (!visited.empty())
        {
            visited.pop_back();
        }

        if (!current_prog.empty())
        {
            current_prog.pop_back();
        }
    };
    
    auto already_visited = [&](const degree_node* node) -> bool 
    {
        return std::find(visited.begin(), visited.end(), node) != visited.end();
    };

    std::function<void(const degree_node*)> explore_node = [&](const degree_node* node) mutable
    {
        visit(node);

        if (node->is_prog_exit() && current_prog.size() > 1)
        {
            _permutations.push_back(current_prog); 
        }
        
        for (const degree_node* next_node : node->get_edges())
        {
            if (!already_visited(next_node))
            {
                explore_node(next_node);
            }
        }

        unvisit_last_node();
    };

    // Actual processing

    for (const degree_node* node : _degrees)
    {
        if (node->is_prog_entry())
        {
            explore_node(node);
        }
    }
}

void mode_graph::print_permutations()
{
    LOG(_permutations.size() << " major permutations");

    int prog_counter = 0;

    for (std::vector<const degree_node*>& progression : _permutations)
    {
        std::cout << std::setw(4) << ++prog_counter << " : ";
        for (const degree_node* degree : progression)
        {
            std::cout << degree->get_name().c_str() << " ";
        }
        std::cout << "\n";
    }
}

}