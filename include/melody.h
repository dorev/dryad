#pragma once

#include "dryadcommon.h"
#include "pattern.h"
#include "dryadutils.h"
#include <numeric>

namespace dryad
{

class mode_graph;

class melody
{
public:

    // A melody can be a pattern like { 0, 0, 2, 4, 8 }
    // jumps of 2 represent arpeggios, 8s are octaves, just like notes of a scale
    // any accidental will come as decoration or later

    melody(size_t min_duration = HALF, size_t max_duration = 2 * WHOLE, size_t min_notes = 2, size_t max_notes = 12);

    inline const pattern& get_notes() const { return _notes; }
    inline const pattern& get_durations() const { return _durations; }

private:

    pattern _notes;
    pattern _durations;

};

}