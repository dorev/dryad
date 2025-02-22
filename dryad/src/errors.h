#pragma once

#include "types.h"

// TODO: make a C enum out of this
constexpr dryad_error dryad_error_success = 0;
constexpr dryad_error dryad_error_not_implemented = 1;
constexpr dryad_error dryad_error_node_not_in_graph = 2;
constexpr dryad_error dryad_error_already_exists = 3;
constexpr dryad_error dryad_error_invalid_voice = 4;
constexpr dryad_error dryad_error_invalid_frame = 5;
constexpr dryad_error dryad_error_invalid_instance = 6;
constexpr dryad_error dryad_error_invalid_motif = 7;
constexpr dryad_error dryad_error_invalid_position = 8;
constexpr dryad_error dryad_error_invalid_motif_note = 9;

const char* dryad_error_string(dryad_error error);
