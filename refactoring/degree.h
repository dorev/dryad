#pragma once

#include "includes.h"

namespace dryad
{
namespace model
{

struct scale;

struct degree
{
    // References
    std::shared_ptr<scale> parent_scale;
};

} // namespace model
} // namespace dryad