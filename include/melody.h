#pragma once

#include "dryadutils.h"
#include "dryadinfo.h"
#include "pattern.h"
#include "note.h"

namespace dryad
{

class mode_graph;

class melody : dryad_info<melody>
{
public:


    melody(int duration = WHOLE, int note_count = 4);

    int get_total_duration() const;

private:

    std::vector<note> _notes;

};

}