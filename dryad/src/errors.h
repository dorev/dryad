#pragma once

#include "types.h"

constexpr dryad_error dryad_no_error = 0;
constexpr dryad_error dryad_not_implemented = 1;
constexpr dryad_error dryad_node_not_in_graph = 2;
constexpr dryad_error dryad_already_exists = 3;

const char* dryad_error_string(dryad_error error)
{
    switch (error)
    {
    default: return "unknown error";
    case dryad_no_error: return "no error";
    case dryad_not_implemented: return "not implemented";
    case dryad_node_not_in_graph: return "node not in graph";
    case dryad_already_exists: return "already exists";
    }
}

