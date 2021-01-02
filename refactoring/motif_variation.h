#pragma once

#include "includes.h"

namespace dryad
{
namespace model
{

class note;
class motif;

class motif_variation
{
    // References
    std::shared_ptr<motif> parent_motif;
    std::vector<note> notes;
};

} // namespace model
} // namespace dryad