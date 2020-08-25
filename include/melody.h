#pragma once

#include "dryadutils.h"
#include "pattern.h"

namespace dryad
{

class mode_graph;

class melody : dryad_info<melody>
{
public:

    melody(int duration = WHOLE, int note_count = 4);

    inline const pattern& get_notes() const     { return _notes; }
    inline const pattern& get_durations() const { return _durations; }

   // static melody

private:

    pattern _notes;
    pattern _durations;

};

}