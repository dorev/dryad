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

    void resize(int target_duration);

private:

    void shrink(int target_duration);
    void extend(int target_duration);

    std::vector<note> _notes;

};

}