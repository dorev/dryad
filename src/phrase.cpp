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

    // Perfect fit! Easy dispatch
    if (prog_size == phrase_size)
    {
        for_range(i, prog_size)
        {
            _bars[i].insert_degree(_progression[i]);
        }
        return;
    }

    // Initialize distribution vector
    std::vector<size_t> degrees_per_bar(phrase_size, 0);

    // Sad values that cannot be initialized in the switch case
    size_t chords_to_fit = prog_size;
    size_t offset = phrase_size;
    size_t bar = 0;

    switch (strategy)
    {
    case fitting_strategy::even_compact_left:
    case fitting_strategy::even_compact_right:

        for (size_t n = 0; n < chords_to_fit; ++n)
        {
            offset = phrase_size;
            bar = 0;

            for_range(bit, log2(phrase_size))
            {
                offset >>= 1;

                bool bit_of_n_is_off = !((1ULL << bit) & n);

                if (bit_of_n_is_off)
                {
                    bar += offset;
                }
            }

            if (prog_size < phrase_size || strategy == fitting_strategy::even_compact_left)
            {
                ++degrees_per_bar[(phrase_size - 1) - bar];
            }
            else if (strategy == fitting_strategy::even_compact_right)
            {
                ++degrees_per_bar[bar];
            }
            else
            {
                CRASH("We should not reach this point")
            }
        }

        goto InsertChords;

    case fitting_strategy::compact_left:
    case fitting_strategy::compact_right:

        while (chords_to_fit != 0)
        {
            for (bar = phrase_size; bar > (phrase_size - offset); --bar)
            {
                if (prog_size < phrase_size || strategy == fitting_strategy::compact_left)
                {
                    ++degrees_per_bar[phrase_size - bar];
                }
                else if (strategy == fitting_strategy::compact_right)
                {
                    ++degrees_per_bar[bar - 1];
                }
                else
                {
                    CRASH("We should not reach this point")
                }

                if (--chords_to_fit == 0)
                {
                    goto InsertChords;
                }
            }

            offset >>= 1;

            if (offset == 0)
            {
                offset = phrase_size;
            }
        }

    default:
        break;
    }

    CRASH("We should not reach this point");

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
        melody_t& melody    = _melodies[0];
        int note_index      = 0;
        int melody_size     = melody.size();

        // currently completely disregarding the delta between the melody duration and the bar duration

        while (add_note(melody[note_index++ % melody_size]));
    }
    else if (_melodies.size() > 1)
    {
        std::vector<int> melody_sizes;

        while (1)
        {
            for (auto& melody : _melodies)
            {
                for (const auto& note : melody)
                {
                    if (!add_note(note))
                    {
                        return;
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool phrase_t::add_note(const note_t& note)
{
    int bar_index = 0;
    bar_t& bar = _bars[bar_index];

    for (; bar_index < _bars.size(); ++bar_index)
    {
        bar = _bars[bar_index];
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

                if (bar_index < (_bars.size() - 1))
                {
                    _bars[bar_index + 1].get_voice().add_note(note.get_offset(), duration_overflow);
                }
                else
                {
                    // There is no next bar and this one is full
                    return false;
                }
            }

            break;
        }
    }

    // Return false if the current bar is full and it's the last bar
    return !(
        bar.get_voice().get_total_duration() >= bar.get_duration() &&
        bar_index >= (_bars.size() - 1)
    );
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