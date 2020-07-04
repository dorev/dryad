#pragma once

#include "dryadcommon.h"

namespace dryad
{

class mode_graph
{
public:

    void print_permutations();
    void generate_permutations(size_t max_prog_length = 8);
    const progression& random_prog(size_t min_length = 0, size_t max_length = 0);

protected:

    std::vector<degree_node*> _degrees;
    std::vector<progression> _progs;

    mode_graph() {}
    virtual ~mode_graph() {}

};

}