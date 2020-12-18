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


        // Access the underlying vector
        note_t& operator[](int index) { return _notes[index]; }
        int size() { return int(_notes.size()); }
        std::vector<note_t>::iterator       begin()     { return _notes.begin(); }
        std::vector<note_t>::iterator       end()       { return _notes.end(); }
        std::vector<note_t>::const_iterator cbegin()    { return _notes.cbegin(); }
        std::vector<note_t>::const_iterator cend()      { return _notes.cend(); }

    private:

        std::vector<note_t> _notes;
    };

}