#pragma once

#include "definitions.h"

namespace dryad
{

struct motif_t;
struct phrase_t;
struct voice_t;

struct score_t
{
    std::vector<motif_ptr> motifs;
    std::vector<voice_ptr> voices;
    std::vector<phrase_ptr> phrases;
    harmony_graph_ptr graph;
};

} // namespace dryad