#include "melody.h"

namespace dryad
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

melody::melody(int duration, int notes_count)
    : _notes()
    , _durations()
{
    // validate duration/notes_count combination
    if (notes_count * __max_duration < duration || notes_count * __min_duration > duration)
    {
        CRASHLOG("Invalid duration/notes_count combination");
    }

    const std::vector<int>& durations_used = __notes_durations;

    PROFILE(dryad_timer timer;)

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

    // helper lambda to ge the current total content size of durations_pattern
    auto total_duration = [&]() -> int
    {
        return std::reduce(durations_pattern.begin(), durations_pattern.end(), 0);
    };

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

        for_range(i, durations_pattern.size())
        {
            if (compare(durations_pattern[i], duration_to_exclude))
            {
                candidates_index.push_back(i);
            }
        }

        int i = random::in(candidates_index);
        durations_pattern[i] = step_function(durations_pattern[i], durations_used);
    }

duration_is_valid:

    // group similar durations to give more sense to the melody


    // set members
    _notes = notes_pattern;
    _durations = durations_pattern;

    PROFILE(LOG("Melody generation took " << timer.stop().c_str());)
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}