#pragma once

#include "includes.h"
#include <memory>

namespace dryad
{
namespace model
{

// Forward declarations
struct degree_t;
struct harmony_graph_t;
struct harmony_node_t;
struct measure_t;
struct motif_t;
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
using motif_variation_weak_ptr   = std::weak_ptr<motif_variation_t>;
using note_weak_ptr              = std::weak_ptr<note_t>;
using phrase_weak_ptr            = std::weak_ptr<phrase_t>;
using position_weak_ptr          = std::weak_ptr<position_t>;
using scale_weak_ptr             = std::weak_ptr<scale_t>;
using score_weak_ptr             = std::weak_ptr<score_t>;
using session_weak_ptr           = std::weak_ptr<session>;
using voice_weak_ptr             = std::weak_ptr<voice_t>;

} // namespace model
} // namespace dryad