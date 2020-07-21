#include "melody.h"

namespace dryad
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

melody::melody(size_t duration, size_t notes_count)
    : _notes()
    , _durations()
{

    std::deque<int> notes_pattern = { 0 };
    std::deque<int> durations_pattern;

    // generate random notes
    // push notes relative to the previous ones
    for_range(i, notes_count - 1)
    {
        notes_pattern.push_back(random::normal_int(static_cast<float>(notes_pattern.back()), 4.0));
    }

    // helper lambda to ge the current total content size of durations_pattern
    auto total_duration = [&]() -> size_t
    {
        return std::reduce(durations_pattern.begin(), durations_pattern.end(), 0);
    };

    // generate initial random durations
    for_range(i, notes_count)
    {
        durations_pattern.push_back(random::in(__notes_durations));
    }

    if (total_duration() != duration)
    {
        // write relevant algo here
        _durations = {};
    }


    // set members
    _notes = notes_pattern;
    _durations = durations_pattern;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}