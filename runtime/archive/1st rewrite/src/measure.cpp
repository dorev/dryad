#include "measure.h"
#include "degreenode.h"
#include "dryadutils.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

measure_t::measure_t()
    : _next(nullptr)
    , _prev(nullptr)
    , _parent(nullptr)
{
    _progression.reserve(MAX_CHORDS_BY_BAR);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

measure_t::measure_t(phrase_t* phrase)
{
    set_parent(phrase);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void measure_t::insert_note(int offset, int duration)
{
    _voice.add_note(offset, duration);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void measure_t::insert_note(const note_t& single_note)
{
    _voice.add_note(single_note);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void measure_t::apply_scale(const scale_t& scale, int root)
{
    // across voices, find out what notes are gonna be affected by what degree
    int measure_duration = static_cast<int>(_voice.get_total_duration());
    int progression_size = static_cast<int>(_progression.size());
    int range_size = 0;
    bool use_last_degree_of_previous_measure = false;

    if (_progression.size() == 0)
    {
        // go get the last degree of the previous measure
        progression_size = 1;
        range_size = 1;
        use_last_degree_of_previous_measure = true;
    }
    else
    {
        progression_size = static_cast<int>(_progression.size());
        range_size = measure_duration / progression_size;
    }

    // get range of notes for each progression
    std::vector<std::pair<int, int>> degree_ranges;

    for_range(i, progression_size)
    {
        degree_ranges.emplace_back
        (
            i * range_size,
            i * range_size + (range_size - 1)
        );
    }

    int cumulative_duration = 0;

    for (note_t& note : _voice)
    {
        // find the degree of this note
        degree_t* degree = nullptr;

        if (use_last_degree_of_previous_measure)
        {
            // TODO:
            // This will be problematic when the previous measure has no chord or doesn't exist
            degree = *(prev()->get_progression().rbegin());
        }
        else
        {
            for_range(i, progression_size)
            {
                if (cumulative_duration >= degree_ranges[i].first &&
                    cumulative_duration <= degree_ranges[i].second)
                {
                    degree = _progression[i];
                    break;
                }
            }
        }

        if (degree == nullptr)
        {
            CRASH("No degree found for note")
        }

        cumulative_duration += note.get_duration();

        // apply note of degree based on root note
        int offset = note.get_offset();
        int anchor = scale[degree->get_degree() - 1];

        note.set_pitch(root + anchor + offset + degree->get_accidental_value());
    }



}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
