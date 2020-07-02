#include "dryadutils.h"

#include <algorithm>
#include <cctype>

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void uppercase(std::string& s)
{
    for (char& c : s)
    {
        c = static_cast<char>(std::toupper(c));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

node_visit_counter::node_visit_counter(degree_node* node) 
    : _node(node)
    , _visit_count(0)
{}

node_visit_counter& node_visit_counter::operator++()
{
    _visit_count++;
    return *this;
}

node_visit_counter& node_visit_counter::operator--()
{
    _visit_count--;
    return *this;
}


bool node_visit_counter::operator<(const node_visit_counter& other) const
{
    return _node < other._node;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}