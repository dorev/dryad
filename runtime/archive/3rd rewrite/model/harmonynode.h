#pragma once

#include "dryad/types.h"

namespace Dryad::Model
{

class Degree;
class HarmonyNodeGroup;

class HarmonyNode
{
    Id _id;
    Degree* _degree;
    Vector<HarmonyNode*> _edges;
    HarmonyNodeGroup* _group;
    Uint _maxVisits;
    Uint _visitCount;
    bool _isEntry;
    bool _isExit;

    inline Id GetId() const { return _id; }
    void AddEdge(HarmonyNode& edge);
    bool IsVisitable();
    void Visit();
    void Leave();

    /*
    void mark_as_entry(std::initializer_list<harmony_node_ptr> nodes, bool value = true);
    void mark_as_exit(std::initializer_list<harmony_node_ptr> nodes, bool value = true);
    void add_edge(harmony_node_ptr node, harmony_node_ptr other_node);
    void add_edges(harmony_node_ptr node, std::initializer_list<harmony_node_ptr> other_nodes);
    bool is_visitable(harmony_node_ptr node);
    void visit(harmony_node_ptr node, std::vector<harmony_node_weak_ptr>& progression);
    void leave(harmony_node_ptr node, std::vector<harmony_node_weak_ptr>& progression);
    */
 
};

} // namespace Dryad::Model