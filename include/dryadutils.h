#pragma once

#include "dryadcommon.h"

#define LOG(x) std::cout << x << "\n"
#define CRASHLOG(x) { std::cout << "\n\n" << x << "\n --> " << __FILE__ << " l." << __LINE__ << "\n\n"; exit(1); }

namespace dryad
{

void uppercase(std::string& s);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class degree_node;

class node_visit_counter
{
public:

    node_visit_counter(degree_node* node);

    node_visit_counter& operator++();
    node_visit_counter& operator--();
    bool operator<(const node_visit_counter& other) const;

private:

    degree_node* _node;
    int _visit_count;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}