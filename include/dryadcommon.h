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
#include <cmath>
#include <chrono>

namespace dryad
{

class degree_node;
using progression = std::vector<degree_node*>;

constexpr size_t MAX_PROG_LENGTH      = 128;
constexpr size_t MAX_BAR_DIVISION     = 4;

// Durations
constexpr size_t WHOLE                = 96;
constexpr size_t HALF                 = 48;
constexpr size_t HALF_TRIPLET         = 32;
constexpr size_t QUARTER              = 24;
constexpr size_t QUARTER_TRIPLET      = 16;
constexpr size_t EIGHTH               = 12;
constexpr size_t EIGHTH_TRIPLET       = 8;
constexpr size_t SIXTEENTH            = 6;
constexpr size_t SIXTEENTH_TRIPLET    = 4;
constexpr size_t THIRTYSECOND         = 3;
constexpr size_t THIRTYSECOND_TRIPLET = 2;


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