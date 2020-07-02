#include "dryadutils.h"

namespace dryad
{

class degree_node
{
public:

    inline int   get_id() const                                 { return _id; }
    inline bool  is_prog_entry() const                          { return _prog_entry; }
    inline bool  is_prog_exit() const                           { return _prog_exit; }
    inline const std::string& get_name() const                  { return _name; }
    inline const std::vector<degree_node*>& get_edges() const   { return _edges; }

    void set_id(int value)                          { _id = value; }
    degree_node& mark_as_entry(bool value = true)   { _prog_entry = value; return *this; }
    degree_node& mark_as_exit (bool value = true)   { _prog_exit = value; return *this; }

    degree_node(int number, int triad, int alteration = 0);

    degree_node& add_edge(std::initializer_list<degree_node*> nodes);

private:

    const int _degree;
    const int _triad;
    const int _alteration;
    int _id;
    bool _prog_entry;
    bool _prog_exit;

    std::string _name;
    std::vector<degree_node*> _edges;

    void build_name();

};

}