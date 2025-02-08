#pragma once

#include "types.h"

constexpr dryad_error dryad_error_success = 0;
constexpr dryad_error dryad_error_not_implemented = 1;
constexpr dryad_error dryad_error_node_not_in_graph = 2;
constexpr dryad_error dryad_error_already_exists = 3;
constexpr dryad_error dryad_error_invalid_voice = 4;
constexpr dryad_error dryad_error_invalid_frame = 5;

const char* dryad_error_string(dryad_error error)
{
    switch (error)
    {
    default: return "unknown error";
    case dryad_error_success: return "no error";
    case dryad_error_not_implemented: return "not implemented";
    case dryad_error_node_not_in_graph: return "node not in graph";
    case dryad_error_already_exists: return "already exists";
    case dryad_error_invalid_voice: return "invalid voice";
    case dryad_error_invalid_frame: return "invalid frame";
    }
}

