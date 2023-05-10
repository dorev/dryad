#pragma once

#include "definitions.h"
#include "monitoring.h"

namespace dryad
{

struct harmony_node_t;
struct measure_t;
struct score_t;

struct phrase_t : monitor_count<phrase_t>
{
    std::vector<measure_ptr> measures;
    std::vector<harmony_node_weak_ptr> progression;
    score_weak_ptr parent_score;
    phrase_weak_ptr next;
    phrase_weak_ptr previous;
};

} // namespace dryad