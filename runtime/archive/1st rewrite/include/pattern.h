#pragma once

#include "dryadutils.h"

namespace dryad
{

class phrase_t;

class pattern
{
public:

    pattern(std::initializer_list<int> pattern = { 0 });
    pattern(const std::deque<int>& pattern);

    inline size_t size() const { return _pattern_size; }
    inline size_t element_count() const { return _element_count; }
    inline const std::deque<int>& value() const { return _pattern; }

private:

    size_t _pattern_size;
    size_t _element_count;
    std::deque<int> _pattern;

};

}