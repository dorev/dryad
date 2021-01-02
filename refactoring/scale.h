#pragma once

#include "includes.h"

namespace dryad
{
namespace model
{

struct degree;

struct scale
{
    // Members
    std::vector<int> intervals;
    std::vector<degree> degrees;
};

} // namespace model
} // namespace dryad