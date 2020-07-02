#pragma once

#include "modegraph.h"
#include "degreenode.h"

namespace dryad
{

class major_graph : public mode_graph
{
public:

    // Ctor
    major_graph();

    // Methods
    void print_permutations() override final;

private:

    degree_node _I;     degree_node* I;
    degree_node _ii;    degree_node* ii;
    degree_node _iii;   degree_node* iii;
    degree_node _IV;    degree_node* IV;
    degree_node _V;     degree_node* V;
    degree_node _vi;    degree_node* vi;
    degree_node _viid;  degree_node* viid;

    std::vector<std::vector<degree_node*>> _permutations;

    void generate_permutations() override final;
};

}