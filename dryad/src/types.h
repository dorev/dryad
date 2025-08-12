#pragma once

#include "overrides.h"
#include <cstdint>

namespace Dryad
{

    using NoteValue = unsigned;
    using NoteRelative = int;
    using BPM = float;
    using Time = unsigned;

    constexpr unsigned InvalidValue = INT32_MAX;

}

#include "errors.h"
