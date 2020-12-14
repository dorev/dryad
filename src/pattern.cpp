#include "pattern.h"
#include "phrase.h"
#include "dryadutils.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pattern::pattern(std::initializer_list<int> pattern)
    : _pattern(pattern)
    , _pattern_size(pattern.size())
    , _element_count(std::set<int>(pattern).size())
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pattern::pattern(const std::deque<int>& pattern)
    : _pattern(pattern)
    , _pattern_size(pattern.size())
    , _element_count(0)
{
    std::set<int> elementCounter;

    for (int i : pattern)
    {
        elementCounter.insert(i);
    }

    _element_count = elementCounter.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}