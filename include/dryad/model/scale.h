#pragma once

#include "dryad/model/definitions.h"

namespace dryad
{

struct degree_t;

struct scale_t
{
    std::string name;
    std::vector<degree_ptr> degrees;
};

} // namespace dryad