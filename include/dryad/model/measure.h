#pragma once

#include "dryad/types.h"
#include "dryad/model/traits.h"
#include "dryad/model/position.h"

namespace Dryad::Model
{

class Measure : public Listed<Measure>
{
    Vector<Position> _positions;
};

} // namespace Dryad::Model