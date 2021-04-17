#pragma once

#include "dryad/types.h"
#include "dryad/model/harmonynodegroup.h"

namespace Dryad::Model
{

class HarmonyGraph
{
    Vector<HarmonyNodeGroup> _nodeGroups;
};

} // namespace Dryad::Model