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
    Id _id;
    Scale _scale;
    Vector<HarmonyNode> nodes;
    Vector<Progression> _progressions;
    HarmonyGraph* _graph;

    inline Id GetId() const { return _id; }
    
    void CalculateProgressions();
};

} // namespace Dryad::Model