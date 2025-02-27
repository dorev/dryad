#pragma once

#include "types.h"

enum dryad_error
{
    dryad_success = 0,
    dryad_error_not_implemented,
    dryad_error_node_not_in_graph,
    dryad_error_already_exists,
    dryad_error_invalid_voice,
    dryad_error_invalid_frame,
    dryad_error_invalid_instance,
    dryad_error_invalid_motif,
    dryad_error_invalid_position,
    dryad_error_invalid_motif_note,
    dryad_error_invalid_edge,
    dryad_error_invalid = -1
};

const char* dryad_error_string(dryad_error error);
