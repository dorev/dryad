#pragma once

#include "dryad/types.h"
#include "dryad/model/degree.h"

namespace Dryad::Model
{

struct ScaleConfig
{
    Vector<ShortInt> intervals;
    Vector<Degree> degrees;
};

} // namespace Dryad::Model