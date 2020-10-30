#pragma once

#include "dryadutils.h"
#include "dryadinfo.h"
#include "voice.h"

namespace dryad
{

class degree_node;

class bar_t : dryad_info<bar_t>
{
public:

    bar_t();

    inline int get_duration() { return WHOLE; }
    inline voice_t& get_voice() { return _voice; }
    inline int get_chords_count() { return int(_progression.size()); }

    void insert_note(int offset, int duration);
    void insert_note(const note_t& single_note);
    inline void insert_degree(degree_node* degree) { _progression.push_back(degree); }

private:

    progression _progression;
    voice_t _voice;
};

}