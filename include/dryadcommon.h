#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <functional>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <chrono>

namespace dryad
{

class degree_node;
using progression = std::vector<degree_node*>;

constexpr size_t MAX_PROG_LENGTH    = 128;
constexpr size_t MAX_BAR_DIVISION   = 4;

enum class accidental
{
    none,
    sharp,
    flat,
};

enum class triad
{
    none,
    major,
    minor,
    dim,
    aug,
};

enum class structural_pattern
{
    none_specified,
    aaaa,
    aaab,
    aabb,
    abab,
    aabc,
    abcc,
    abac,
    abcd,
};

enum class fitting_strategy
{
    compact_left,
    compact_right,
    symmetric_far,
    symmetric_close,
    even_compact_left,
    even_compact_right,
    even_symmetric_far,
    even_symmetric_close,
};

}