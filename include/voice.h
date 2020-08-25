#pragma once

#include "dryadutils.h"
#include "note.h"

namespace dryad
{
    class voice : dryad_info<voice>
    {
    public:

        void add_note(note note);
        void add_note(int offset, int duration);

    private:

        std::vector<note> _notes;
    };

}