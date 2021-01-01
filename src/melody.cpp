#include "melody.h"

namespace dryad
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

melody_t::melody_t(int duration, int notes_count)
{
    // validate duration/notes_count combination
    if (notes_count * __max_duration < duration || notes_count * __min_duration > duration)
    {
        CRASH("Invalid duration/notes_count combination");
    }

    const std::vector<int>& durations_used = __notes_durations;

    std::deque<int> notes_pattern = { 0 };
    std::deque<int> durations_pattern;

    // generate random notes
    // push notes relative to the previous ones
    for_range(i, notes_count - 1)
    {
        notes_pattern.push_back(random::normal_int(static_cast<float>(notes_pattern.back()), 4.0));
    }

    // generate initial random durations
    for_range(i, notes_count)
    {
        durations_pattern.push_back(random::in(durations_used));
    }

    // helper lambda to get the current total content size of durations_pattern
    auto total_duration = [&]() -> int
    {
        return std::reduce(durations_pattern.begin(), durations_pattern.end());
    };

    // helper lambda for comparaisons in the duration fitting loop
    auto greater = [](int lhs, int rhs) -> bool { return lhs > rhs; };
    auto lesser = [](int lhs, int rhs) -> bool { return lhs < rhs; };

    // fit durations to match requested total duration
    while (int duration_to_fill = (duration - total_duration()))
    {
        bool need_to_grow = duration_to_fill > 0;

        // can a single duration change fix it?
        for(int& duration_value : durations_pattern)
        {
            if (!need_to_grow &&
                std::abs(duration_to_fill) == (duration_value - step_down_duration(duration_value, durations_used)))
            {
                duration_value = step_down_duration(duration_value, durations_used);
                goto duration_is_valid;
            }
            else if (need_to_grow &&
                std::abs(duration_to_fill) == (step_up_duration(duration_value, durations_used) - duration_value))
            {
                duration_value = step_up_duration(duration_value, durations_used);
                goto duration_is_valid;
            }
        }

        // when we will look at current durations_pattern values, if we want to increase the total duration we'll
        // exclude the two highest durations possible from the selected candidates for incrementation, on the other
        // hand if we want to decrease the duration, we'll exclude the two smallers durations possible from the
        // selected candidates for decrementation
        int duration_to_exclude = need_to_grow
                ? step_down_duration(__max_duration, durations_used)
                : step_up_duration(__min_duration, durations_used);

        // select the duration step function according to if we're increasing of decreasing total duration
        auto step_function = need_to_grow ? step_up_duration : step_down_duration;

        // set function to compare against duration_to_exclude
        std::function<bool(int, int)> compare;
        if (need_to_grow)
        {
            compare = lesser;
        }
        else
        {
            compare = greater;
        }

        // prepare a vector to store the indexes of values that are still incrementable
        std::vector<int> candidates_index;
        candidates_index.reserve(notes_count);

        for (;;)
        {
            for_range(i, durations_pattern.size())
            {
                if (compare(durations_pattern[i], duration_to_exclude))
                {
                    candidates_index.push_back(i);
                }
            }

            // if no current duration seems like a reasonable one to step
            if (candidates_index.size() == 0)
            {
                // loosen duration exclusions
                int next_duration_to_exclude = step_function(duration_to_exclude, durations_used);

                // if we are already at the limit of available durations
                if (next_duration_to_exclude == duration_to_exclude)
                {
                    CRASH("Loosening duration exclusions not sufficient to fit duration/notes_count combination");
                }
                else
                {
                    duration_to_exclude = next_duration_to_exclude;
                }
            }
            else
            {
                // we have some room to work with
                break;
            }
        }


        int i = random::in(candidates_index);
        durations_pattern[i] = step_function(durations_pattern[i], durations_used);
    }

duration_is_valid:

    // TODO?
    // group similar durations to give more sense to the melody

    // set members

    for_range(i, notes_count)
    {
        _notes.emplace_back(notes_pattern[i], durations_pattern[i]);
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int melody_t::get_total_duration() const
{
    int result =  std::reduce(
        _notes.begin(), _notes.end(), 0,
        [](int acc, const note_t& note)
        {
            return acc + note.get_duration();
        });

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void melody_t::resize(int target_duration)
{
    if ((target_duration - get_total_duration()) > 0)
    {
        extend(target_duration);
    }
    else
    {
        shrink(target_duration);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void melody_t::shrink(int target_duration)
{
    int duration = get_total_duration();

    if (target_duration > duration)
    {
        CRASH("Cannot shrink to a longer duration");
    }

    // the last note of the melody is removed until we reach the
    // target duration, then the last note is ajusted

    switch (random::range(0, 1))
    {
    case 0:
 
        // extend last note once melody is too short
 
        while (duration > target_duration && _notes.size() > 1)
        {
            duration -= _notes.back().get_duration();
            _notes.pop_back();
        }

        _notes.back().extend_duration(target_duration - duration);

        break;

    case 1:

        // shrink last overflowing note

        while (_notes.size() > 0)
        {
            int next_duration = get_total_duration() - _notes.back().get_duration();

            if (next_duration < target_duration)
            {
                _notes.back().set_duration(target_duration - next_duration);
                return;
            }
            else
            {
                _notes.pop_back();
                if (next_duration == target_duration)
                {
                    return;
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void melody_t::extend(int target_duration)
{
    int duration = get_total_duration();

    if (target_duration < duration)
    {
        CRASH("Cannot extend to a shorter duration");
    }

    int note_index = 0;
    int melody_size = int(_notes.size());

    // TODO :   Add a something to have a shift between the repetitions and
    //          the original to give a sense of movement

    switch (random::range(0, 1))
    {
    case 0:

        // repeat from beginning

        while (duration < target_duration)
        {
            _notes.push_back(_notes[note_index++ % melody_size]);
            duration += _notes.back().get_duration();
        }

        break;

    case 1:

        // repeat backwards (mirror)
        int last_index = melody_size - 1;

        while (duration < target_duration)
        {
            _notes.push_back(_notes[last_index - (note_index++ % melody_size)]);
            duration += _notes.back().get_duration();
        }

        break;
    }

    // fix duration overflow
    if (duration > target_duration)
    {
        shrink(target_duration);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



}