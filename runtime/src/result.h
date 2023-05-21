#pragma once

#include "types.h"
#include "flags.h"

namespace Dryad
{
    DRYAD_DECLARE_FLAG_ENUM(Result, UInt32)
    {
        DRYAD_FLAG(Success, 0),
        DRYAD_FLAG(EmptyResult, 1),
        DRYAD_FLAG(UnspecifiedError, 2),
        DRYAD_FLAG(NotFound, 3),
        DRYAD_FLAG(UnsupportedEventType, 4),
        DRYAD_FLAG(InvalidEventData, 5),
        DRYAD_FLAG(InvalidEventType, 6),
        DRYAD_FLAG(NotYetImplemented, 7),
        DRYAD_FLAG(ConcurrencyError, 8),
        DRYAD_FLAG(UselessCall, 9),
        DRYAD_FLAG(EdgeNotFound, 10),
        DRYAD_FLAG(NodeNotFound, 11),
        DRYAD_FLAG(GraphNotFound, 12),
        DRYAD_FLAG(ContainerEmpty, 13),
    };

    inline Result ResetResult(Result& result)
    {
        return result = Result::EmptyResult;
    }
}

