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
        DRYAD_FLAG(PotentialConcurrencyError, 8),
        DRYAD_FLAG(UselessOperation, 9),
        DRYAD_FLAG(EdgeNotFound, 10),
        DRYAD_FLAG(NodeNotFound, 11),
        DRYAD_FLAG(GraphNotFound, 12),
        DRYAD_FLAG(ContainerEmpty, 13),
        DRYAD_FLAG(InvalidGraph, 14),
        DRYAD_FLAG(InvalidNode, 15),
        DRYAD_FLAG(InvalidTime, 16),
        DRYAD_FLAG(InvalidEdge, 17),
        DRYAD_FLAG(HarmonyFrameNotFound, 18),
        DRYAD_FLAG(FailedToTransition, 19),
    };

    inline Result ResetResult(Result& result)
    {
        return result = Result::EmptyResult;
    }
}

