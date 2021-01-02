#pragma once

#include "includes.h"

namespace dryad
{
namespace model
{

struct motif_variation;

struct motif
{
    // Members
    std::vector<motif_variation> variations;
};

} // namespace model
} // namespace dryad