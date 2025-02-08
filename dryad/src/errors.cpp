#include "errors.h"

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
    case dryad_error_invalid_instance: return "invalid instance";
    }
}

