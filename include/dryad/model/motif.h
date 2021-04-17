#pragma once

#include "dryad/types.h"
#include "dryad/model/motifvariation.h"

namespace Dryad::Model
{

class Motif
{
    Vector<MotifVariation> _variations;
};

} // namespace Dryad::Model