#pragma once

#include "dryadutils.h"

namespace dryad
{

class mode_graph 
{
public:

    void print_permutations();
    void generate_permutations(size_t max_prog_length = 8);
    const progression& random_prog(size_t min_length = 0, size_t max_length = 0);

    inline std::vector<degree_node*>&   get_degrees() { return _degrees; }
    inline std::vector<progression>&    get_progs()   { return _progs; }
    inline const std::vector<int>&      get_scale()   { return _scale; }

protected:

    std::vector<degree_node*> _degrees;
    std::vector<progression> _progs;
    const std::vector<int> _scale;

    mode_graph(std::vector<int> scale);
    virtual ~mode_graph() {}

};

}