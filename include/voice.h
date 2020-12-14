#pragma once

#include "dryadutils.h"
#include "dryadinfo.h"
#include "note.h"

namespace dryad
{
    class voice_t : dryad_info<voice_t>
    {
    public:

        void add_note(note_t note);
        void add_note(int offset, int duration);

        int get_total_duration();

    private:

        std::vector<note_t> _notes;
    };

}