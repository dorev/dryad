#pragma once

#include "includes.h"

namespace dryad
{
namespace model
{

struct score;

struct voice
{
    // Members
    std::string name;

    // References
    std::shared_ptr<score> parent_score;
};

} // namespace model
} // namespace dryad