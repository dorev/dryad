#pragma once

#include "includes.h"

namespace dryad
{

using u16 = short unsigned;
using u32 = unsigned;
using u64 = unsigned long long;

class degree_t;
class phrase_t;
using progression_t = std::vector<degree_t*>;
using scale_t = std::vector<int>;
using song_t = std::vector<phrase_t>;


constexpr size_t MAX_PROG_LENGTH      = 16;
constexpr size_t MAX_CHORDS_BY_BAR    = 4;

// Durations
constexpr int WHOLE                = 96;
constexpr int HALF_DOTTED          = 72;
constexpr int HALF                 = 48;
constexpr int QUARTER_DOTTED       = 36;
constexpr int HALF_TRIPLET         = 32;
constexpr int QUARTER              = 24;
constexpr int EIGHTH_DOTTED        = 18;
constexpr int QUARTER_TRIPLET      = 16;
constexpr int EIGHTH               = 12;
constexpr int EIGHTH_TRIPLET       = 8;
constexpr int SIXTEENTH            = 6;
constexpr int SIXTEENTH_TRIPLET    = 4;
constexpr int THIRTYSECOND         = 3;
constexpr int THIRTYSECOND_TRIPLET = 2;

static const std::vector<int> __notes_durations
({
//    THIRTYSECOND,
//    SIXTEENTH,
    EIGHTH,
//    EIGHTH_DOTTED,
    QUARTER,
    QUARTER_DOTTED,
    HALF,
//    HALF_DOTTED,
//    WHOLE
});

static const int __min_duration = EIGHTH;
static const int __max_duration = HALF;

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
    double_sharp,
    flat,
    double_flat,
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
    none,
    random,
    compact_left,
    compact_right,
    even_compact_left,
    even_compact_right,
};

#define MAKE_MIDI_OCTAVE(octave)                  \
constexpr int C##octave     = octave * 12;       \
constexpr int Cs##octave    = octave * 12 + 1;   \
constexpr int Df##octave    = octave * 12 + 1;   \
constexpr int D##octave     = octave * 12 + 2;   \
constexpr int Ds##octave    = octave * 12 + 3;   \
constexpr int Ef##octave    = octave * 12 + 3;   \
constexpr int E##octave     = octave * 12 + 4;   \
constexpr int F##octave     = octave * 12 + 5;   \
constexpr int Fs##octave    = octave * 12 + 6;   \
constexpr int Gf##octave    = octave * 12 + 6;   \
constexpr int G##octave     = octave * 12 + 7;   \
constexpr int Gs##octave    = octave * 12 + 8;   \
constexpr int Af##octave    = octave * 12 + 8;   \
constexpr int A##octave     = octave * 12 + 9;   \
constexpr int As##octave    = octave * 12 + 10;  \
constexpr int Bf##octave    = octave * 12 + 10;  \
constexpr int B##octave     = octave * 12 + 11;

MAKE_MIDI_OCTAVE(0)
MAKE_MIDI_OCTAVE(1)
MAKE_MIDI_OCTAVE(2)
MAKE_MIDI_OCTAVE(3)
MAKE_MIDI_OCTAVE(4)
MAKE_MIDI_OCTAVE(5)
MAKE_MIDI_OCTAVE(6)
MAKE_MIDI_OCTAVE(7)
MAKE_MIDI_OCTAVE(8)
MAKE_MIDI_OCTAVE(9)

#undef MAKE_MIDI_OCTAVE






}