#pragma once

#include "dryad/types.h"
#include "dryad/model/traits.h"

namespace Dryad::Model
{

class FunctionalNote;

class ConcreteNote : public Listed<ConcreteNote>
{

public:

    ConcreteNote();

    INLINE_GET_SET(Uint, Note, _note)
    INLINE_GET_SET(Uint, Octave, _octave)
    INLINE_GET_SET(Uint, Duration, _duration)
    INLINE_GET_SET(FunctionalNote*, FunctionalNote, _functionalNote)

private:

    Uint _note;
    Uint _octave;
    Uint _duration;
    FunctionalNote* _functionalNote;
};

} // namespace Dryad::Model