#pragma once

#include <memory>

namespace dryad
{
namespace model
{

// Forward declarations
struct degree;
struct harmony_graph;
struct harmony_node;
struct measure;
struct motif;
struct motif_variation;
struct note;
struct phrase;
struct position;
struct scale;
struct score;
struct session;
struct voice;

// Pointer aliases
using degree_ptr = std::shared_ptr<degree>;
using harmony_graph_ptr = std::shared_ptr<harmony_graph>;
using harmony_node_ptr = std::shared_ptr<harmony_node>;
using measure_ptr = std::shared_ptr<measure>;
using motif_ptr = std::shared_ptr<motif>;
using motif_variation_ptr = std::shared_ptr<motif_variation>;
using note_ptr = std::shared_ptr<note>;
using phrase_ptr = std::shared_ptr<phrase>;
using position_ptr = std::shared_ptr<position>;
using scale_ptr = std::shared_ptr<scale>;
using score_ptr = std::shared_ptr<score>;
using session_ptr = std::shared_ptr<session>;
using voice_ptr = std::shared_ptr<voice>;

} // namespace model
} // namespace dryad