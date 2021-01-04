#pragma once

#include "model_definitions.h"
namespace dryad
{
namespace model
{

struct position_t;
struct phrase_t;
struct degree_t;

struct measure_t
{
    std::vector<position_ptr> positions;
    std::vector<degree_ptr> associated_degrees;
    phrase_weak_ptr parent_phrase;
    measure_weak_ptr next;
    measure_weak_ptr prev;
};

} // namespace model
} // namespace dryad