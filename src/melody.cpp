#include "melody.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

melody::melody(size_t min_duration, size_t max_duration, size_t min_notes, size_t max_notes)
    : _notes()
    , _durations()
{
    // validating input values
    if (max_duration < min_duration ||
        max_notes < min_notes)
    {
        CRASHLOG("Invalid min/max values");
    }

    if (__min_duration * max_notes < min_duration ||
        __max_duration * min_notes > max_duration)
    {
        CRASHLOG("Invalid combination of note count and melody duration");
    }

    size_t notes_count = random::range(min_notes, max_notes);

    std::deque<int> notes_pattern = { 0 };
    std::deque<int> durations_pattern;

    // generate random notes
    // push notes relative to the previous ones
    for_range(i, notes_count - 1)
    {
        notes_pattern.push_back(random::normal_int(static_cast<float>(notes_pattern.back()), 2.0));
    }

    // helper lambda to ge the current total content size of durations_pattern
    auto total_duration = [&]() -> size_t
    {
        return std::reduce(durations_pattern.begin(), durations_pattern.end(), 0);
    };

    // generate random durations for melody notes
    for_range(i, notes_count)
    {
        // 50% chance to input the same duration than previously
        if (i != 0 &&
            random::coin_flip() &&
            (total_duration() + durations_pattern.back()) < max_duration)
        {
            durations_pattern.push_back(durations_pattern.back());
        }
        else
        {
            // insert random duration among 
            size_t duration_index = random::range(0, __notes_durations.size() - 1);

            // work to get a valid duration
            for (;;)
            {
                if ((total_duration() + __notes_durations[duration_index]) > max_duration)
                {
                    if (duration_index != 0)
                    {
                        --duration_index;
                    }
                    else
                    {
                        // the pattern contains too long durations

                        // find a previous duration value that is not the smallest one and reduce it
                        for (;;)
                        {
                            // brute & inefficient approach, but let's keep going forward

                            // get reference to a random value in durations_pattern
                            int& dur = random::in(durations_pattern);

                            // if it's not the smallest duration existing
                            if (dur != __min_duration)
                            {
                                step_down_duration(dur, __notes_durations);
                                break;
                            }
                        }
                    }
                    continue;
                }

                break;
            }

            durations_pattern.push_back(__notes_durations[duration_index]);
        }
    }

    if (total_duration() < min_duration)
    {
        // inflate some notes
        for (;;)
        {
            // brute & inefficient approach, but let's keep going forward

            // get reference to a random value in durations_pattern
            int& dur = random::in(durations_pattern);

            // if it's not the smallest duration existing
            if (dur != __max_duration)
            {
                step_up_duration(dur, __notes_durations);
                break;
            }
        }
    }

    // set members
    _notes = notes_pattern;
    _durations = durations_pattern;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}