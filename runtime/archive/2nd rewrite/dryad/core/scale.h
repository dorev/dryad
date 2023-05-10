#pragma once

#include "definitions.h"
#include "monitoring.h"

namespace dryad
{

struct degree_t;

struct scale_t : monitor_count<scale_t>
{
    std::string name;
    std::vector<degree_ptr> degrees;
};

} // namespace dryad