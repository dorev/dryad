#pragma once

#include "dryad/types.h"
#include "dryad/model/traits.h"

namespace Dryad::Model
{

class Degree;
class Voice;
class Position;
class ConcreteNote;

class FunctionalNote : public Listed<FunctionalNote>
{

public:

    INLINE_GET_SET(Int, Offset, _offset)
    INLINE_GET_SET(Uint, Duration, _duration)
    INLINE_GET_SET(Degree*, Degree, _degree)
    INLINE_GET_SET(Voice*, Voice, _voice)

private:

    Int _offset;
    Uint _duration;
    Degree* _degree;
    Voice* _voice;
    Position* _position;
    ConcreteNote* _concreteNote;
};

} // namespace Dryad::Model