#pragma once

#include "dryad/types.h"

namespace Dryad::Model
{

class Degree;
class HarmonyNodeGroup;

class HarmonyNode
{
    Degree* _degree;
    HarmonyNode* _edges;
    HarmonyNodeGroup* _group;
};

} // namespace Dryad::Model