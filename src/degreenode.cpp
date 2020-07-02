#include "degreenode.h"

namespace dryad
{

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

    if (_alteration != 0)
    {
        _name += _alteration == FLAT ? "b" : "#";
    }
}

degree_node::degree_node(int number, int triad, int alteration)
    : _degree(number)
    , _triad(triad)
    , _alteration(alteration)
    , _id(0)
    , _prog_entry(false)
    , _prog_exit(false)
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

degree_node& degree_node::add_edge(std::initializer_list<degree_node*> nodes)
{
    _edges.insert(_edges.end(), nodes);
    return *this;
}

}