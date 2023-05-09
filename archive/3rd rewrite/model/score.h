#pragma once

#include "dryad/model/harmonygraph.h"
#include "dryad/model/voice.h"
#include "dryad/model/motif.h"
#include "dryad/model/phrase.h"

namespace Dryad::Model
{

class Score
{
    HarmonyGraph _harmonyGraph;
    Vector<Voice> _voices;
    Vector<Motif> _motifs;
    Vector<Phrase> _phrases;
    /*
    score_ptr create_score();
    void relink_score(score_ptr score);
    void relink_phrase(phrase_ptr phrase);
    void apply_scale(score_ptr score, scale_ptr scale, scale_config_ptr scale_config);
    harmony_node_ptr resolve_harmony_node(position_ptr position);
    */
};

} // namespace Dryad::Model