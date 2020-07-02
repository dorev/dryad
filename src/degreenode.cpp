#include "degreenode.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

degree_node::degree_node(int number, int triad, int alteration)
    : _degree(number)
    , _triad(triad)
    , _alteration(alteration)
    , _id(0)
    , _prog_entry(false)
    , _prog_exit(false)
    , _visit_count(0)
    , _max_visit(1)
{
    if (_degree < 1 || _degree > 7)
    {
        CRASHLOG("Unsupported degree value : " << _degree);
    }

    if (_triad < MAJOR || _triad > AUG)
    {
        CRASHLOG("Unsupported triad value : " << _triad);
    }

    if (_alteration < FLAT || _alteration > SHARP)
    {
        CRASHLOG("Unsupported alteration value : " << _degree);
    }

    build_name();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void degree_node::build_name()
{
    switch (_degree)
    {
    case 1: _name = "i"; break;
    case 2: _name = "ii"; break;
    case 3: _name = "iii"; break;
    case 4: _name = "iv"; break;
    case 5: _name = "v"; break;
    case 6: _name = "vi"; break;
    case 7: _name = "vii"; break;
    default:
        CRASHLOG("Unsupported degree value : " << _degree);
        break;
    }

    if (_triad == MAJOR || _triad == AUG)
    {
        uppercase(_name);
    }

    if (_triad == DIM)
    {
        _name += "'";
    }

    if (_triad == AUG)
    {
        _name += "+";
    }
    

    if (_alteration != 0)
    {
        _name += _alteration == FLAT ? "b" : "#";
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

degree_node& degree_node::add_edge(std::initializer_list<degree_node*> nodes)
{
    _edges.insert(_edges.end(), nodes);
    return *this;
}

void degree_node::visit()
{
    if (!is_visitable())
    {
        CRASHLOG("A node should not be visited if max_visit limit is exceeded");
    }

    _visit_count++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void degree_node::leave()
{
    if (_visit_count - 1 < 0)
    {
        CRASHLOG("A node should not be leaved more than it was visited");
    }

    _visit_count--;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

degree_node& degree_node::mark_as_entry(bool value)
{
    _prog_entry = value;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

degree_node& degree_node::mark_as_exit(bool value)
{
    _prog_exit = value;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}