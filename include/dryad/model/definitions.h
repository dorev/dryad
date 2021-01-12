#pragma once

#include "includes.h"

#define DEBUG_BREAK(x) \
{ \
    std::cout << "\n\n" << x << "\n --> " << __FILE__ << " l." << __LINE__ << "\n\n"; \
    *(reinterpret_cast<unsigned int*>(0xBAAAAADBADBADBAD)) = 0U; \
    throw; \
}

namespace dryad
{

// Forward declarations
struct degree_t;
struct harmony_graph_t;
struct harmony_node_t;
struct measure_t;
struct motif_t;
struct motif_config_t;
struct motif_variation_t;
struct note_t;
struct phrase_t;
struct position_t;
struct scale_t;
struct scale_config_t;
struct score_t;
struct session;
struct voice_t;

// Shared pointer aliases
using degree_ptr            = std::shared_ptr<degree_t>;
using harmony_graph_ptr     = std::shared_ptr<harmony_graph_t>;
using harmony_node_ptr      = std::shared_ptr<harmony_node_t>;
using measure_ptr           = std::shared_ptr<measure_t>;
using motif_ptr             = std::shared_ptr<motif_t>;
using motif_config_ptr      = std::shared_ptr<motif_config_t>;
using motif_variation_ptr   = std::shared_ptr<motif_variation_t>;
using note_ptr              = std::shared_ptr<note_t>;
using phrase_ptr            = std::shared_ptr<phrase_t>;
using position_ptr          = std::shared_ptr<position_t>;
using scale_ptr             = std::shared_ptr<scale_t>;
using scale_config_ptr      = std::shared_ptr<scale_config_t>;
using score_ptr             = std::shared_ptr<score_t>;
using session_ptr           = std::shared_ptr<session>;
using voice_ptr             = std::shared_ptr<voice_t>;

// Weak pointer aliases
using degree_weak_ptr            = std::weak_ptr<degree_t>;
using harmony_graph_weak_ptr     = std::weak_ptr<harmony_graph_t>;
using harmony_node_weak_ptr      = std::weak_ptr<harmony_node_t>;
using measure_weak_ptr           = std::weak_ptr<measure_t>;
using motif_weak_ptr             = std::weak_ptr<motif_t>;
using motif_config_weak_ptr      = std::weak_ptr<motif_config_t>;
using motif_variation_weak_ptr   = std::weak_ptr<motif_variation_t>;
using note_weak_ptr              = std::weak_ptr<note_t>;
using phrase_weak_ptr            = std::weak_ptr<phrase_t>;
using position_weak_ptr          = std::weak_ptr<position_t>;
using scale_weak_ptr             = std::weak_ptr<scale_t>;
using scale_config_weak_ptr      = std::weak_ptr<scale_config_t>;
using score_weak_ptr             = std::weak_ptr<score_t>;
using session_weak_ptr           = std::weak_ptr<session>;
using voice_weak_ptr             = std::weak_ptr<voice_t>;

// Durations
constexpr int _whole_dotted_         = 144;
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

static const std::vector<int> allowed_rhythmic_fractions =
{
    _sixteenth_,
    _eighth_,
    _quarter_,
    _quarter_dotted_,
    _half_,
    _half_dotted_,
    _whole_,
    _whole_dotted_
};

// Intervals
constexpr int _minor_second_        = 1;
constexpr int _second_              = 2;
constexpr int _minor_third_         = 3;
constexpr int _third_               = 4;
constexpr int _fourth_              = 5;
constexpr int _augmented_fourth_    = 6;
constexpr int _fifth_               = 7;
constexpr int _minor_sixth_         = 8;
constexpr int _sixth_               = 9;
constexpr int _seventh_             = 10;
constexpr int _major_seventh_       = 11;

// Progression fitting modes
enum class fitting_mode_e
{
    power_of_2_left,
    power_of_2_right,
    compact_left,
    compact_right
};

enum class accidental_e
{
    none,
    sharp,
    flat
};

// Notes
static const char* _C_     = "C";
static const char* _Cs_    = "C#";
static const char* _Df_    = "Db";
static const char* _D_     = "D";
static const char* _Ds_    = "D#";
static const char* _Ef_    = "Eb";
static const char* _E_     = "E";
static const char* _F_     = "F";
static const char* _Fs_    = "F#";
static const char* _Gf_    = "Gb";
static const char* _G_     = "G";
static const char* _Gs_    = "G#";
static const char* _Af_    = "Ab";
static const char* _A_     = "A";
static const char* _As_    = "A#";
static const char* _Bf_    = "Bb";
static const char* _B_     = "B";

static const std::vector<int> _base_notes_ = {0, 2, 4, 5, 7, 9, 11};
static const std::vector<const char*> _note_names_sharp_ = { _C_, _Cs_, _D_, _Ds_, _E_, _F_, _Fs_, _G_, _Gs_, _A_, _As_, _B_};
static const std::vector<const char*> _note_names_flat_ = { _C_, _Df_, _D_, _Ef_, _E_, _F_, _Gf_, _G_, _Af_, _A_, _Bf_, _B_};

// Chord
static const std::vector<int> _M7_  = { 4, 7, 10 };
static const std::vector<int> _MM7_ = { 4, 7, 11 };
static const std::vector<int> _m7_  = { 3, 7, 10 };
static const std::vector<int> _mM7_ = { 3, 7, 11 };
static const std::vector<int> _dim_ = { 3, 6 };
static const std::vector<int> _aug_ = { 4, 8 };

} // namespace dryad