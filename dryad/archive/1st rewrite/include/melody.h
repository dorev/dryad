#pragma once

#include "dryadutils.h"
#include "dryadinfo.h"
#include "pattern.h"
#include "note.h"

namespace dryad
{

class mode_graph;

class melody_t : dryad_info<melody_t>
{
public:

    melody_t(int duration = WHOLE, int note_count = 4);

    int get_total_duration() const;
    void resize(int target_duration);
    void shrink(int target_duration);
    void extend(int target_duration);

    // Access the underlying vector
    note_t& operator[](int index) { return _notes[index]; }
    int size() { return int(_notes.size()); }
    std::vector<note_t>::iterator       begin()  { return _notes.begin(); }
    std::vector<note_t>::iterator       end()    { return _notes.end(); }
    std::vector<note_t>::const_iterator cbegin() { return _notes.cbegin(); }
    std::vector<note_t>::const_iterator cend()   { return _notes.cend(); }

private:

    std::vector<note_t> _notes;

};

}