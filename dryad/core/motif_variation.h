#pragma once

#include "definitions.h"
#include "monitoring.h"

namespace dryad
{

struct note_t;
struct motif_t;

struct motif_variation_t : monitor_count<motif_variation_t>
{
    motif_weak_ptr parent_motif;
    std::vector<note_ptr> notes;
};

} // namespace dryad