#pragma once

#include "definitions.h"

namespace dryad
{

struct degree_t;

struct scale_config_t
{
    scale_config_t()
        : root(0)
        , accidental(accidental_e::sharp)
    {}

    int root;
    accidental_e accidental;
};

} // namespace dryad