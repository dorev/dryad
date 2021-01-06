#pragma once

#include "includes.h"
#include <memory>

namespace dryad
{

// Forward declarations
struct degree_t;
struct harmony_graph_t;
struct harmony_node_t;
struct measure_t;
struct motif_t;
struct motif_energy_t;
struct motif_variation_t;
struct note_t;
struct phrase_t;
struct position_t;
struct scale_t;
struct score_t;
struct session;
struct voice_t;

// Pointer aliases
using degree_ptr            = std::shared_ptr<degree_t>;
using harmony_graph_ptr     = std::shared_ptr<harmony_graph_t>;
using harmony_node_ptr      = std::shared_ptr<harmony_node_t>;
using measure_ptr           = std::shared_ptr<measure_t>;
using motif_ptr             = std::shared_ptr<motif_t>;
using motif_energy_ptr      = std::shared_ptr<motif_energy_t>;
using motif_variation_ptr   = std::shared_ptr<motif_variation_t>;
using note_ptr              = std::shared_ptr<note_t>;
using phrase_ptr            = std::shared_ptr<phrase_t>;
using position_ptr          = std::shared_ptr<position_t>;
using scale_ptr             = std::shared_ptr<scale_t>;
using score_ptr             = std::shared_ptr<score_t>;
using session_ptr           = std::shared_ptr<session>;
using voice_ptr             = std::shared_ptr<voice_t>;

using degree_weak_ptr            = std::weak_ptr<degree_t>;
using harmony_graph_weak_ptr     = std::weak_ptr<harmony_graph_t>;
using harmony_node_weak_ptr      = std::weak_ptr<harmony_node_t>;
using measure_weak_ptr           = std::weak_ptr<measure_t>;
using motif_weak_ptr             = std::weak_ptr<motif_t>;
using motif_energy_weak_ptr      = std::weak_ptr<motif_energy_t>;
using motif_variation_weak_ptr   = std::weak_ptr<motif_variation_t>;
using note_weak_ptr              = std::weak_ptr<note_t>;
using phrase_weak_ptr            = std::weak_ptr<phrase_t>;
using position_weak_ptr          = std::weak_ptr<position_t>;
using scale_weak_ptr             = std::weak_ptr<scale_t>;
using score_weak_ptr             = std::weak_ptr<score_t>;
using session_weak_ptr           = std::weak_ptr<session>;
using voice_weak_ptr             = std::weak_ptr<voice_t>;

// Durations
constexpr int _whole_                = 96;
constexpr int _half_dotted_          = 72;
constexpr int _half_                 = 48;
constexpr int _quarter_dotted_       = 36;
constexpr int _half_triplet_         = 32;
constexpr int _quarter_              = 24;
constexpr int _eighth_dotted_        = 18;
constexpr int _quarter_triplet_      = 16;
constexpr int _eighth_               = 12;
constexpr int _eighth_triplet_       = 8;
constexpr int _sixteenth_            = 6;
constexpr int _sixteenth_triplet_    = 4;
constexpr int _thirtysecond_         = 3;
constexpr int _thirtysecond_triplet_ = 2;

// Intervals
constexpr int _minor_second_ = 1;
constexpr int _second_ = 2;
constexpr int _minor_third_ = 3;
constexpr int _third_ = 4;
constexpr int _fourth_ = 5;
constexpr int _augmented_fourth_ = 6;
constexpr int _fifth_ = 7;
constexpr int _minor_sixth_ = 8;
constexpr int _sixth_ = 9;
constexpr int _minor_seventh_ = 10;
constexpr int _seventh_ = 11;

// Chord
constexpr std::initializer_list<int> _M7_  = {0, 4, 7, 10};
constexpr std::initializer_list<int> _MM7_ = {0, 4, 7, 11};
constexpr std::initializer_list<int> _m7_  = {0, 3, 7, 10};
constexpr std::initializer_list<int> _mM7_ = {0, 3, 7, 10};
constexpr std::initializer_list<int> _dim_ = {0, 3, 6};
constexpr std::initializer_list<int> _aug_ = {0, 4, 8};

} // namespace dryad