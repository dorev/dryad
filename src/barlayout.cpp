#include "barlayout.h"
#include "dryadutils.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bar_layout::bar_layout()
    : _valid(false)
    , _pattern(structural_pattern::none)
{
    _degrees.reserve(MAX_BAR_DIVISION);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool bar_layout::arrange(std::vector<degree_node*> degrees, structural_pattern pattern)
{
    // Validate that number of degrees fit with the pattern given
    switch (degrees.size())
    {
    case 1: 
        if (pattern != structural_pattern::none)
        {
            return _valid = false;
        }
        break;

    case 2:
        if (pattern != structural_pattern::aaab &&
            pattern != structural_pattern::aabb &&
            pattern != structural_pattern::abab)
        {
            return _valid = false;
        }
        break;

    case 3:
        if (pattern != structural_pattern::aabc &&
            pattern != structural_pattern::abcc &&
            pattern != structural_pattern::abac)
        {
            return _valid = false;
        }
        break;

    case 4:
        if (pattern != structural_pattern::abcd)
        {
            return _valid = false;
        }
        break;

    default:
        CRASHLOG("Unable to divide properly the bar for " << degrees.size() << " degrees");
    }

    // Size of degrees and pattern fit together
    // Set the internal values
    _degrees = degrees;
    _pattern = pattern;

    return _valid = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}