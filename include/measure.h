#pragma once

#include "dryadutils.h"
#include "dryadinfo.h"
#include "voice.h"

namespace dryad
{

class degree_t;
class phrase_t;

class measure_t : dryad_info<measure_t>
{
public:

    measure_t();
    measure_t(phrase_t* phrase);

    inline int get_duration() { return WHOLE; }
    inline voice_t& get_voice() { return _voice; }
    inline int get_chords_count() { return int(_progression.size()); }

    void insert_note(int offset, int duration);
    void insert_note(const note_t& single_note);
    inline void insert_degree(degree_t* degree) { _progression.push_back(degree); }
    inline progression_t& get_progression() { return _progression; }

    void apply_scale(const scale_t& scale_intervals, int root);
    measure_t* next() { return _next; }
    measure_t* prev() { return _prev; }
    inline void set_parent(phrase_t* parent) { _parent = parent; }

private:

    progression_t _progression;
    voice_t _voice;
    measure_t* _next;
    measure_t* _prev;
    phrase_t* _parent;
};

}