#pragma once

#include "definitions.h"
#include "monitoring.h"

namespace dryad
{

struct position_t;
struct phrase_t;
struct harmony_node_t;

struct measure_t : monitor_count<measure_t>
{
    measure_t(int duration = _whole_)
        : duration(duration)
    {}

    int duration;
    std::vector<position_ptr> positions;
    std::vector<harmony_node_ptr> progression;
    phrase_weak_ptr parent_phrase;
    measure_weak_ptr next;
    measure_weak_ptr previous;
};

} // namespace dryad