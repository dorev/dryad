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
};

} // namespace Dryad::Model