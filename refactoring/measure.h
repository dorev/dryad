#pragma once

#include "definitions.h"
namespace dryad
{

struct position_t;
struct phrase_t;
struct harmony_node_t;

struct measure_t
{
    std::vector<position_ptr> positions;
    std::vector<harmony_node_ptr> progression;
    phrase_weak_ptr parent_phrase;
    measure_weak_ptr next;
    measure_weak_ptr prev;
};

} // namespace dryad