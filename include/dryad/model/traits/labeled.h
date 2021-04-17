#pragma once

#include "dryad/types.h"

namespace Dryad::Model
{

class Labeled
{

public:

    INLINE_GET_SET(Uint, Number, _number)
    INLINE_GET_SET(const String&, Name, _name)

protected:

    Uint _number;
    String _name;
};

} // namespace Dryad::Model