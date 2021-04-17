#pragma once

#include "dryad/types.h"
#include <utility>

namespace Dryad
{

template <class... Args>
void Log(Cstr format, Args... args)
{
    printf(format, std::forward<Args>(args)...);
}

}