#pragma once

#include "dryadcommon.h"
#include "barlayout.h"

namespace dryad
{

class phrase_layout
{
public:

    phrase_layout(size_t bar_count = 4, structural_pattern preffered_pattern = structural_pattern::none_specified);

    void apply_progression(const progression& degrees);

private:

    structural_pattern _preffered_pattern;
    std::vector<degree_node*> _degrees;
    std::vector<std::vector<bar_layout>> _bars;

};

}