#pragma once

#include "dryadutils.h"
#include "dryadinfo.h"
#include "bar.h"
#include "pattern.h"
#include "melody.h"

namespace dryad
{

class phrase_t : dryad_info<phrase_t>
{
public:

    phrase_t(size_t bar_count = 4);

    void add_melody(const melody_t& melody);

    inline void set_progression(const progression& progression) { _progression = progression; }
    inline const progression& get_progression() const { return _progression; }

    void fit_progression(fitting_strategy strategy = fitting_strategy::even_compact_right);
    void fit_melodies(fitting_strategy strategy = fitting_strategy::random);

    bar_t& operator[](size_t index);
    inline size_t size() { return _bars.size(); }

private:

    void add_note(const note_t& note);

    std::vector<bar_t> _bars;
    std::vector<melody_t> _melodies;
    progression _progression;

};

}