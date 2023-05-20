#pragma once

#include "types.h"
#include "flags.h"

namespace Dryad
{
    enum class Result
    {
        FLAG(Success, 0),
        FLAG(EmptyResult, 1),
        FLAG(UnspecifiedError, 2),
        FLAG(NotFound, 3),
        FLAG(UnsupportedEventType, 4),
        FLAG(InvalidEventData, 5),
        FLAG(InvalidEventType, 6),
        FLAG(NotYetImplemented, 7),
        FLAG(ConcurrencyError, 8),
        FLAG(UselessCall, 9),
    };
}

