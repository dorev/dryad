#pragma once

#include "includes.h"

namespace dryad
{
namespace model
{

struct degree;
struct measure;
struct score;

struct phrase
{
    // Members
    std::vector<measure> mesures;

    // References
    std::vector<std::shared_ptr<degree>> associated_degrees;
    std::shared_ptr<score> parent_score;
};

} // namespace model
} // namespace dryad