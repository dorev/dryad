#include "pattern.h"
#include "phraselayout.h"
#include "dryadutils.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pattern::pattern(std::initializer_list<int> pattern_representation)
    : _pattern(pattern_representation)
    , _size(pattern_representation.size())
    , _element_count(0)
{
    std::set<int> tmp(pattern_representation);
    _element_count = tmp.size();

    // make sure that pattern is only composed of consecutive int starting from 0
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void pattern::apply(const progression& prog, phrase_layout& phrase, fitting_strategy strategy)
{
    size_t prog_size = prog.size();
    size_t phrase_size = phrase.size();

    // Perfect case
    if (prog_size == _element_count && phrase_size == _size)
    {
        size_t phrase_index = 0;

        for (size_t element : _pattern)
        {
            phrase[phrase_index++].insert(prog[element]);
        }

        return;
    }

    switch (strategy)
    {
    case fitting_strategy::surprise_me:
        return;
    case fitting_strategy::compact_left:
        return;
    case fitting_strategy::compact_right:
        return;
    case fitting_strategy::symmetric_far:
        return;
    case fitting_strategy::symmetric_close:
        return;
    case fitting_strategy::even_compact_left:
        return;
    case fitting_strategy::even_compact_right:
        return;
    case fitting_strategy::even_symmetric_far:
        return;
    case fitting_strategy::even_symmetric_close:
        return;
    default:
        break;
    }

    CRASHLOG("Derp! Not supported yet!");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}