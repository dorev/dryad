#pragma once

#include "types.h"

constexpr dryad_error dryad_no_error = 0;
constexpr dryad_error dryad_not_implemented = 1;

dryad_string dryad_error_string(dryad_error error)
{
    switch (error)
    {
    default: return "unknown error";
    case dryad_no_error: return "no error";
    case dryad_not_implemented: return "not implemented";
    }
}

