#pragma once

#include "dryad/types.h"
#include "dryad/model/scale.h"
#include "dryad/model/harmonynode.h"
#include "dryad/model/progression.h"

namespace Dryad::Model
{

class HarmonyGraph;

class HarmonyNodeGroup
{
    Scale _scale;
    Vector<HarmonyNode> _nodes;
    Vector<Progression> _progressions;
    HarmonyGraph* _graph;
};

} // namespace Dryad::Model