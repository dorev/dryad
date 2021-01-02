#pragma once

#include "includes.h"

namespace dryad
{
namespace model
{

struct position;
struct phrase;
struct degree;

struct measure
{
    // Members
    std::vector<position> positions;

    // References
    std::vector<std::shared_ptr<degree>> associated_degrees;
    std::shared_ptr<phrase> parent_phrase;
};

} // namespace model
} // namespace dryad