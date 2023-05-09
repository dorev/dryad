#pragma once

#include "dryad/types.h"
#include "dryad/model/harmonynodegroup.h"

namespace Dryad::Model
{

class HarmonyGraph
{

public:

    void AddNodeGroup();
    void AddNodeGroup(HarmonyNodeGroup& harmonyNodeGroup);
    HarmonyNodeGroup& GetNodeGroup(Id groupId);

private:

    Vector<HarmonyNodeGroup> _nodeGroups;

    /*
    harmony_graph_ptr create_major_graph();
    void add_node(harmony_graph_ptr graph, harmony_node_ptr node);
    void add_nodes(harmony_graph_ptr graph, std::initializer_list<harmony_node_ptr> nodes);
    void generate_progressions(harmony_graph_ptr graph);
    */
};

} // namespace Dryad::Model