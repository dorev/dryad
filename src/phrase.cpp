#include "phrase.h"
#include "dryadutils.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

phrase::phrase(size_t bar_count, fitting_strategy fitting_strategy)
    : _fitting_strategy(fitting_strategy)
{
    _bars.reserve(bar_count);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void phrase::apply_progression(const progression& prog)
{
    size_t prog_size = prog.size();
    size_t phrase_size = _bars.capacity();

    // Perfect fit!
    if (prog_size == phrase_size)
    {

    }
    else if (prog_size > phrase_size)
    {
        switch (_fitting_strategy)
        {
        case fitting_strategy::even_compact_right:

            return;

        case fitting_strategy::compact_left:
        case fitting_strategy::compact_right:
        case fitting_strategy::symmetric_far:
        case fitting_strategy::symmetric_close:
        case fitting_strategy::even_compact_left:
        case fitting_strategy::even_symmetric_far:
        case fitting_strategy::even_symmetric_close:
        default:
            break;
        }
    }
    else// if (prog_size < phrase_size)
    {
        switch (_fitting_strategy)
        {
        case fitting_strategy::even_compact_right:

            return;

        case fitting_strategy::compact_left:
        case fitting_strategy::compact_right:
        case fitting_strategy::symmetric_far:
        case fitting_strategy::symmetric_close:
        case fitting_strategy::even_compact_left:
        case fitting_strategy::even_symmetric_far:
        case fitting_strategy::even_symmetric_close:
        default:
            break;
        }
    }

    CRASHLOG("Not implemented yet!");

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bar& phrase::operator[](size_t index)
{
    if (index > _bars.size() - 1)
    {
        CRASHLOG("Out of bound");
    }

    return _bars[index];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}