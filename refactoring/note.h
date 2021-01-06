#pragma once

#include "definitions.h"

namespace dryad
{

struct position_t;
struct voice_t;
struct motif_t;

struct note_t
{
    note_t(int offset = 0)
        : offset(offset)
        , duration(0)
        , alteration(0)
        , octave(0)
        , step("C")
    {}

    int offset;
    int duration;
    int alteration;
    int octave;
    const char* step;
    voice_ptr associated_voice;
    motif_ptr associated_motif;
    position_weak_ptr parent_position;
};

} // namespace dryad