#pragma once

#include "dryadcommon.h"
#include "bar.h"
#include "pattern.h"

namespace dryad
{

class phrase
{
public:

    phrase(size_t bar_count = 4, fitting_strategy fitting_strategy = fitting_strategy::even_compact_right);

    inline fitting_strategy get_fitting_strategy() const { return _fitting_strategy; }

    void apply_progression(const progression& prog);
    bar& operator[](size_t index);

private:

    fitting_strategy _fitting_strategy;
    std::vector<bar> _bars;

};

}