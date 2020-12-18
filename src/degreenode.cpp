#include "degreenode.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

degree_t::degree_t(int number, triad triad, accidental accidental)
    : _degree(number)
    , _triad(triad)
    , _accidental(accidental)
    , _id(0)
    , _prog_entry(false)
    , _prog_exit(false)
    , _visit_count(0)
    , _max_visit(1)
{
    if (_degree < 1 || _degree > 7)
    {
        CRASH("Unsupported degree value : " << _degree);
    }

    build_name();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void degree_t::build_name()
{
    switch (_degree)
    {
    case 1: _name = "i";    break;
    case 2: _name = "ii";   break;
    case 3: _name = "iii";  break;
    case 4: _name = "iv";   break;
    case 5: _name = "v";    break;
    case 6: _name = "vi";   break;
    case 7: _name = "vii";  break;
    default:
        CRASH("Unsupported degree value : " << _degree);
        break;
    }

    switch (_triad)
    {
    case triad::major:
        uppercase(_name);
        break;
    
    case triad::minor:
        break;

    case triad::dim:
        _name += "'";
        break;

    case triad::aug:
        uppercase(_name);
        _name += "+";
        break;

    default:
        CRASH("Unsupported triad value : " << static_cast<int>(_triad));
        break;
    }

    switch (_accidental)
    {
    case accidental::none:
        break;

    case accidental::flat:
        _name += "b";
        break;

    case accidental::sharp:
        _name += "#";
        break;

    default:
        CRASH("Unsupported accidental value : " << static_cast<int>(_accidental));
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

degree_t& degree_t::add_edge(std::initializer_list<degree_t*> nodes)
{
    _edges.insert(_edges.end(), nodes);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void degree_t::visit()
{
    if (!is_visitable())
    {
        CRASH("A node should not be visited if max_visit limit is exceeded");
    }

    _visit_count++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void degree_t::leave()
{
    if (_visit_count - 1 < 0)
    {
        CRASH("A node should not be leaved more than it was visited");
    }

    _visit_count--;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

degree_t& degree_t::mark_as_entry(bool value)
{
    _prog_entry = value;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

degree_t& degree_t::mark_as_exit(bool value)
{
    _prog_exit = value;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int degree_t::get_accidental_value() const
{
    switch (_accidental)
    {
        default:
        case accidental::none:          return 0;
        case accidental::sharp:         return 1;
        case accidental::double_sharp:  return 2;
        case accidental::flat:          return -1;
        case accidental::double_flat:   return -2;
    }
}

}