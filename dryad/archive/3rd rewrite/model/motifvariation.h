#pragma once

#include "dryad/types.h"
#include "dryad/model/functionalnote.h"

namespace Dryad::Model
{

class Motif;

class MotifVariation
{
    Vector<FunctionalNote> _notes;
    Motif* _motif;
};

} // namespace Dryad::Model