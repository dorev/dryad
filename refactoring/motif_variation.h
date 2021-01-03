#pragma once

#include "model_definitions.h"

namespace dryad
{
namespace model
{

class note_t;
class motif_t;

class motif_variation_t
{
    motif_weak_ptr parent_motif;
    std::vector<note_ptr> notes;
};

} // namespace model
} // namespace dryad