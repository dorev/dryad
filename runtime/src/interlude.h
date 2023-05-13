#pragma once

#include "types.h"

namespace Dryad
{
    struct Interlude
    {
        GraphPtr graph;
        MotifPtr melody;
    };

    using InterludePtr = SharedPtr<Interlude>;
}