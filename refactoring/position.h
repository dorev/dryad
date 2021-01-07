#pragma once

#include "definitions.h"

namespace dryad
{

struct harmony_node_t;
struct measure_t;
struct note_t;

struct position_t
{
    position_t()
        : measure_time(0)
    {}

    int measure_time;
    std::vector<note_ptr> notes;
    harmony_node_ptr harmony_node;
    measure_weak_ptr parent_measure;
    position_weak_ptr next;
    position_weak_ptr prev;
};

} // namespace dryad