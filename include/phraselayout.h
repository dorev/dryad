#pragma once

#include "dryadcommon.h"
#include "barlayout.h"

namespace dryad
{

class phrase_layout
{
public:

    phrase_layout(size_t bar_count = 4, structural_pattern preffered_pattern = structural_pattern::none_specified);

    inline size_t size() const { return _bars.capacity(); }

    void apply_progression(const progression& degrees);

    bar_layout& operator[](size_t index);

private:

    structural_pattern _preffered_pattern;
    std::vector<bar_layout> _bars;

};

}