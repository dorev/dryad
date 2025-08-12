#pragma once

#include "types.h"

namespace Dryad
{

    enum Error
    {
        Success = 0,
        NotImplemented,
        NodeNotInGraph,
        AlreadyExists,
        InvalidVoice,
        InvalidFrame,
        InvalidMotifInstance,
        InvalidMotif,
        InvalidPosition,
        InvalidMotifNote,
        InvalidEdge,
        InvalidDegree,
        InvalidScale,
        Invalid = -1
    };

    const char* ToString(Error error);

}
