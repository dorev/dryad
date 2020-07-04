#pragma once

#include "dryadcommon.h"

namespace dryad
{

class degree_node;

class bar_layout
{
public:

    bar_layout();

    inline bool is_valid() const { return _valid; }

    bool arrange(std::vector<degree_node*> degrees, structural_pattern pattern = structural_pattern::aaaa);

    inline void insert(degree_node* degree) { _degrees.push_back(degree); }

private:

    bool                        _valid;
    structural_pattern          _pattern;
    std::vector<degree_node*>   _degrees;

};

}