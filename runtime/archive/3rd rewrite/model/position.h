#pragma once

#include "dryad/types.h"
#include "dryad/model/functionalnote.h"
#include "dryad/model/traits.h"

namespace Dryad::Model
{

class Position : public Listed<Position>
{
    Vector<FunctionalNote> _notes;
};

} // namespace Dryad::Model