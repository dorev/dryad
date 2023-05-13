#pragma once

#include "types.h"

namespace Dryad
{
    enum Result : UInt8
    {
        Success,
        UnspecifiedError,
        NotFound,
        InvalidArgument,
        NotImplemented,
    };
}
