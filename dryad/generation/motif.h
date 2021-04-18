#pragma once

#include "dryad/types.h"
#include "dryad/model.h"

namespace Dryad::Generation
{

using namespace Model;

void GenerateMotif(Motif& motif, MotifConfig& config, Uint variationCount = 1);

} // namespace Dryad::Generation