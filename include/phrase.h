#pragma once

#include "dryadutils.h"
#include "dryadinfo.h"
#include "measure.h"
#include "pattern.h"
#include "melody.h"

namespace dryad
{

class phrase_t : dryad_info<phrase_t>
{
public:

    phrase_t(size_t bar_count = 4);

    void add_melody(const melody_t& melody);

    void fit_progression(fitting_strategy strategy = fitting_strategy::even_compact_right);
    void fit_melodies(fitting_strategy strategy = fitting_strategy::random);

    inline const progression_t& get_progression() const { return _progression; }
    inline void set_progression(const progression_t& progression_t) { _progression = progression_t; }
    inline std::vector<melody_t>& get_melodies() { return _melodies; }
    inline std::vector<measure_t>& get_measures() { return _measures; }
    inline void set_parent(song_t* parent) { _parent = parent; }
    inline size_t size() { return _measures.size(); }

    void apply_scale(const scale_t& scale, midi root);
    measure_t& operator[](size_t index);

private:

    bool add_note(const note_t& note);

    std::vector<measure_t> _measures;
    std::vector<melody_t> _melodies;
    progression_t _progression;
    phrase_t* _prev;
    phrase_t* _next;
    song_t* _parent;

};

}