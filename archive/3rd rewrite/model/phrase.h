#pragma once

#include "dryad/types.h"
#include "dryad/model/progression.h"
#include "dryad/model/measure.h"
#include "dryad/model/traits.h"

namespace Dryad::Model
{

class Phrase : public Listed<Phrase>
{
    Progression _progression;
    Vector<Measure> _measures;
    /*
    void apply_progression(phrase_ptr phrase, const std::vector<harmony_node_weak_ptr>& progression, fitting_mode_e fitting_mode = fitting_mode_e::power_of_2_right);
    void apply_motif(phrase_ptr phrase, motif_variation_ptr motif_variation, voice_ptr voice);
    void append_phrase(score_ptr score, phrase_ptr phrase);
    void append_measure(phrase_ptr phrase, measure_ptr measure);
    void append_note(measure_ptr measure, note_ptr note);
    */
};

} // namespace Dryad::Model