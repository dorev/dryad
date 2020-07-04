#pragma once

#include "dryadcommon.h"
#include "dryadutils.h"

namespace dryad
{

class degree_node
{

public:

    degree_node(int number, triad triad, accidental accidental = accidental::none);

    inline int    get_id() const                                { return _id; }
    inline size_t get_visit_count() const                       { return _visit_count; }
    inline bool   is_visitable() const                          { return _visit_count < _max_visit; }
    inline bool   is_prog_entry() const                         { return _prog_entry; }
    inline bool   is_prog_exit() const                          { return _prog_exit; }
    inline const  std::string& get_name() const                 { return _name; }
    inline const  std::vector<degree_node*>& get_edges() const  { return _edges; }

    inline void set_id(int value)           { _id = value; }
    inline void set_max_visit(int value)    { _max_visit = value; }
    void visit();
    void leave();
    degree_node& mark_as_entry(bool value = true);
    degree_node& mark_as_exit(bool value = true);
    degree_node& add_edge(std::initializer_list<degree_node*> nodes);

private:

    const int                   _degree;
    const triad                 _triad;
    const accidental            _accidental;
    int                         _id;
    bool                        _prog_entry;
    bool                        _prog_exit; 
    size_t                      _visit_count;
    size_t                      _max_visit;
    std::string                 _name;
    std::vector<degree_node*>   _edges;

    void build_name();

};

}