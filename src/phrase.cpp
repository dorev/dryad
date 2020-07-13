#include "phrase.h"
#include "dryadutils.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

phrase::phrase(size_t bar_count, fitting_strategy fitting_strategy)
    : _fitting_strategy(fitting_strategy)
    , _bars(bar_count)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void phrase::apply_progression(const progression& prog)
{
    size_t prog_size = prog.size();
    size_t phrase_size = _bars.capacity();

    if (!is_power_of_2(phrase_size))
    {
        CRASHLOG("A phrase should be a power of 2");
    }

    // Perfect fit!
    if (prog_size == phrase_size)
    {
        for (size_t i = 0; i < prog_size; ++i)
        {
            _bars[i].insert(prog[i]);
        }
        return;
    }

    int base_degrees_per_bar = prog_size / phrase_size;
    std::vector<size_t> degrees_per_bar(phrase_size, base_degrees_per_bar);

    // Sad values that cannot be initialized in the switch case
    size_t last_bar = phrase_size - 1;
    size_t n = 0;
    size_t bar = 0;
    
    if (prog_size > phrase_size)
    {
        size_t exceeding_chords = prog_size % phrase_size;
        size_t bars_to_fill     = phrase_size;

        switch (_fitting_strategy)
        {
        case fitting_strategy::even_compact_right:

            for (n; n < exceeding_chords; ++n)
            {
                size_t offset = phrase_size;
                bar = 0;

                for (size_t lsb = 0; lsb < log2(phrase_size); ++lsb)
                {
                    offset /= 2;

                    bool bit_of_n_is_off = !((1 << lsb) & n);
                    if (bit_of_n_is_off)
                    {
                        bar += offset;
                    }
                }

                ++degrees_per_bar[bar];
            }

            goto InsertChords;

        case fitting_strategy::compact_left:

            while (exceeding_chords != 0)
            {
                bars_to_fill /= 2;

                for (bar = last_bar; bar >= (phrase_size - bars_to_fill); --bar)
                {
                    ++degrees_per_bar[bar];
                    if (!--exceeding_chords)
                    {
                        goto InsertChords;
                    }
                }
            }
            break;

        case fitting_strategy::compact_right:
        case fitting_strategy::even_compact_left:
        default:
            break;
        }



    }
    else if (prog_size < phrase_size)
    {
        switch (_fitting_strategy)
        {
        case fitting_strategy::even_compact_right:

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

InsertChords:

    size_t prog_index = 0;

    for (bar = 0; bar < phrase_size; ++bar)
    {
        while (degrees_per_bar[bar]--)
        {
            _bars[bar].insert(prog[prog_index++]);
        }
    }
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