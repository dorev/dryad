#pragma once

#include "dryadutils.h"

namespace dryad
{

class mode_graph 
{
public:

    void print_permutations();
    void generate_permutations(size_t max_prog_length = 8);
    const progression_t& random_prog(size_t min_length = 0, size_t max_length = 0);

    inline progression_t&   get_degrees() { return _degrees; }
    inline std::vector<progression_t>&    get_progs()   { return _progs; }
    inline const scale_t&      get_scale()   { return _scale; }

protected:

    progression_t _degrees;
    std::vector<progression_t> _progs;
    const scale_t _scale;

    mode_graph(scale_t scale_intervals);
    virtual ~mode_graph() {}

};

}