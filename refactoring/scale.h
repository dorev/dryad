#pragma once

#include "model_definitions.h"

namespace dryad
{
namespace model
{

struct degree_t;

struct scale_t
{
    std::string name;
    std::vector<degree_ptr> degrees;
};

} // namespace model
} // namespace dryad