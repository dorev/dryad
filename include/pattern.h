#pragma once

#include "dryadcommon.h"

namespace dryad
{

class phrase_layout;

enum class fitting_strategy
{
    surprise_me,
    compact_left,
    compact_right,
    symmetric_far,
    symmetric_close,
    even_compact_left,
    even_compact_right,
    even_symmetric_far,
    even_symmetric_close,
};

class pattern
{
public:

    pattern(std::initializer_list<int> pattern = { 0 });

    inline size_t size() const { return _size; }
    inline size_t element_count() const { return _element_count; }

    void apply(const progression& prog, phrase_layout& phrase, fitting_strategy strategy = fitting_strategy::surprise_me);

private:

    size_t _size;
    size_t _element_count;
    std::vector<int> _pattern;

};

}