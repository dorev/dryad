#pragma once

#include "dryad/types.h"
#include "dryad/model/progression.h"
#include "dryad/model/measure.h"
#include "dryad/model/traits.h"

namespace Dryad::Model
{

class Phrase : public Listed<Phrase>
{
    Progression _progression;
    Vector<Measure> _measures;
};

} // namespace Dryad::Model