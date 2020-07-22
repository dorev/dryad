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

    melody(int duration = WHOLE, int note_count = 4);

    inline const pattern& get_notes() const     { return _notes; }
    inline const pattern& get_durations() const { return _durations; }

private:

    pattern _notes;
    pattern _durations;

};

}