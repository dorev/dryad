#pragma once

#include "types.h"

namespace dryad
{
    template <class T>
    T Min(const T& a, const T& b)
    {
        return a < b ? a : b;
    }

    template <class T>
    T Max(const T& a, const T& b)
    {
        return a < b ? b : a;
    }
}