#pragma once

#include "dryadutils.h"

namespace dryad
{

class degree_node;
using progression = std::vector<degree_node*>;

constexpr size_t MAX_PROG_LENGTH      = 128;
constexpr size_t MAX_CHORDS_BY_BAR    = 4;

// Durations
constexpr int WHOLE                = 96;
constexpr int HALF                 = 48;
constexpr int HALF_TRIPLET         = 32;
constexpr int QUARTER              = 24;
constexpr int QUARTER_TRIPLET      = 16;
constexpr int EIGHTH               = 12;
constexpr int EIGHTH_TRIPLET       = 8;
constexpr int SIXTEENTH            = 6;
constexpr int SIXTEENTH_TRIPLET    = 4;
constexpr int THIRTYSECOND         = 3;
constexpr int THIRTYSECOND_TRIPLET = 2;

static const int __min_duration = THIRTYSECOND;
static const int __max_duration = WHOLE;

static const std::vector<int> __notes_durations
({
    THIRTYSECOND,
    SIXTEENTH,
    EIGHTH,
    QUARTER,
    HALF,
    WHOLE
});

static const std::vector<int> __triplets_durations
({
    THIRTYSECOND_TRIPLET,
    SIXTEENTH_TRIPLET,
    EIGHTH_TRIPLET,
    QUARTER_TRIPLET,
    HALF_TRIPLET
});

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