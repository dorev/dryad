#include "phrase.h"
#include "dryadutils.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

phrase_t::phrase_t(size_t bar_count)
    : _bars(bar_count)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void phrase_t::fit_progression(fitting_strategy strategy)
{
    size_t prog_size = _progression.size();
    size_t phrase_size = _bars.capacity();

    if (!is_power_of_2(phrase_size))
    {
        CRASH("A phrase should be a power of 2");
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

    size_t base_degrees_per_bar = prog_size / phrase_size;
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

                    bool bit_of_n_is_off = !((1ULL << bit) & n);

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

    CRASH("Not implemented yet!");

InsertChords:

    size_t prog_index = 0;

    for_range(i, phrase_size)
    {
        // For the number of chords in that measure
        while (degrees_per_bar[i]--)
        {
            // Insert the next chord of the progression in that measure
            _bars[i].insert_degree(_progression[prog_index++]);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void phrase_t::add_melody(const melody_t& melody)
{
    _melodies.push_back(melody);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void phrase_t::fit_melodies(fitting_strategy /*strategy*/)
{
    // If we have a single melody
    if (_melodies.size() == 1)
    {
        melody_t& melody = _melodies[0];
        int melody_duration = melody.get_total_duration();
        //int bar_duration = _bars[0].get_duration();

        // Resize melody to fit the closest bar count
        bool round_up = melody_duration % WHOLE > HALF;
        int target_duration = int(melody_duration / WHOLE) + round_up ? 1 : 0;

        melody_t melody_clone(melody);
        melody_clone.resize(target_duration);

        while (1) // until all the bars are filled
        {
        }
    }
    else if (_melodies.size() > 1)
    {
        int combined_melodies_duration = std::reduce(_melodies.begin(), _melodies.end(), 0,
            [](int acc, const melody_t& melody)
            {
                return acc + melody.get_total_duration();
            });


        if (is_power_of_2(combined_melodies_duration))
        {
            // Write melody notes to the bars

        }
        else
        {
            // If it is smaller or bigger
                // Write an altered version to fit the size of PROGRESSION CHORD (could make a feature switch later to fit the bar rather than the chords)
        }

    }
    else // (_melodies.size() < 1)
    {
        CRASH("Invalid phrase melodies size");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void phrase_t::add_note(const note_t& note)
{
    for (int i = 0; i < _bars.size(); ++i)
    {
        bar_t& bar = _bars[i];
        int voice_duration = bar.get_voice().get_total_duration();
        int bar_duration = bar.get_duration();

        if (voice_duration < bar_duration)
        {
            int delta = bar_duration - voice_duration;
            int note_duration = note.get_duration();

            if (note_duration <= delta)
            {
                bar.get_voice().add_note(note);
            }
            else
            {
                int duration_overflow = note_duration - delta;
                bar.get_voice().add_note(note.get_offset(), delta);

                if (i < (_bars.size() - 1))
                {
                    _bars[i + 1].get_voice().add_note(note.get_offset(), duration_overflow);
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bar_t& phrase_t::operator[](size_t index)
{
    if (index > _bars.size() - 1)
    {
        CRASH("Out of bound");
    }

    return _bars[index];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}