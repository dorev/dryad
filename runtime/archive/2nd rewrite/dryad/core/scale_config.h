#pragma once

#include "definitions.h"

namespace dryad
{

struct degree_t;

struct scale_config_t
{
    scale_config_t(int root = 0, accidental_e accidental = accidental_e::sharp)
        : root(root)
        , accidental(accidental)
    {}

    int root;
    accidental_e accidental;
};

} // namespace dryad