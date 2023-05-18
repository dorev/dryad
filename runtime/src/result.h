#pragma once

#include "types.h"

namespace Dryad
{
    enum Result : UInt8
    {
        Success,
        UnspecifiedError,
        NotFound,
        UnsupportedEventType,
        InvalidEventData,
        InvalidEventType,
        NotYetImplemented,
        ConcurrencyError,
    };
}
