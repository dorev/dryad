#pragma once

#include "scale.h"

struct Degree
{
    int _degree;
    Scale _scale;

    Degree()
        : _degree(0)
        , _scale()
    {
    }

    Degree(int degree, Scale scale = Scale())
        : _degree(degree)
        , _scale(scale)
    {
    }

    bool operator<(const Degree &other)
    {
        return _degree < other._degree;
    }
}