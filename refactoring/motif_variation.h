#pragma once

#include "model_definitions.h"

namespace dryad
{
namespace model
{

struct note_t;
struct motif_t;

struct motif_variation_t
{
    motif_weak_ptr parent_motif;
    std::vector<note_ptr> notes;
};

} // namespace model
} // namespace dryad