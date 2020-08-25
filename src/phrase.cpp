#include "phrase.h"
#include "dryadutils.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

phrase::phrase(size_t bar_count)
    : _bars(bar_count)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void phrase::fit_progression(fitting_strategy strategy)
{
    size_t prog_size = _progression.size();
    size_t phrase_size = _bars.capacity();

    if (!is_power_of_2(phrase_size))
    {
        CRASHLOG("A phrase should be a power of 2");
    }

    // Perfect fit!
    if (prog_size == phrase_size)
    {
        for_range(i, prog_size)
        {
            _bars[i].insert_degree(_progression[i]);
        }
        return;
    }

    int base_degrees_per_bar = prog_size / phrase_size;
    std::vector<size_t> degrees_per_bar(phrase_size, base_degrees_per_bar);

    // Sad values that cannot be initialized in the switch case
    size_t last_bar = phrase_size - 1;
    size_t n = 0;
    size_t bar = 0;
    
    // If we have more chords than measures, apply shrinking version of fitting strategy
    if (prog_size > phrase_size)
    {
        size_t exceeding_chords = prog_size % phrase_size;
        size_t bars_to_fill     = phrase_size;

        switch (strategy)
        {
        case fitting_strategy::even_compact_right:

            for (n; n < exceeding_chords; ++n)
            {
                size_t offset = phrase_size;
                bar = 0;

                for_range(bit, log2(phrase_size))
                {
                    offset /= 2;

                    bool bit_of_n_is_off = !((1 << bit) & n);

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
    // If we have less chords than measures, apply expanding version of fitting strategy
    else if (prog_size < phrase_size)
    {
        switch (strategy)
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

    for_range(i, phrase_size)
    {
        // for the number of chords in that measure
        while (degrees_per_bar[i]--)
        {
            // insert the next chord of the progression in that measure
            _bars[i].insert_degree(_progression[prog_index++]);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void phrase::add_melody(const melody& melody)
{
    _melodies.push_back(melody);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void phrase::fit_melodies(fitting_strategy /*strategy*/)
{
    // if we have a single melody
        // if melody size is different that the size of a bar
            // if it is smaller or bigger
                // write an altered version to fit the size of PROGRESSION CHORD (could make a feature switch later to fit the bar rather than the chords)


        // or if it has the same size of a bar
            // write melody notes to the bar

    // if we have more than one melody



    // how do we write the melodies to a bar?
    // do we make a list by voice of melodies

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