#pragma once

#include "definitions.h"

namespace dryad
{

struct degree_t;
struct measure_t;
struct note_t;

struct position_t
{
    std::vector<note_ptr> notes;
    degree_ptr associated_degree;
    measure_ptr parent_measure;
    position_weak_ptr next;
    position_weak_ptr prev;
};

} // namespace dryad