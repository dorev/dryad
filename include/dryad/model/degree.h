#pragma once

#include "dryad/model/traits.h"

namespace Dryad::Model
{

class Degree : public Labeled, public Listed<Degree>
{
};

} // namespace Dryad::Model