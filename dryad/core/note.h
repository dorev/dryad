#pragma once

#include "definitions.h"
#include "monitoring.h"

namespace dryad
{

struct position_t;
struct voice_t;
struct motif_t;

struct note_t : monitor_count<note_t>
{
    note_t(int offset = 0)
        : offset(offset)
        , duration(0)
        , midi(-1)
        , accidental(accidental_e::none)
        , octave(-1)
        , step(nullptr)
        , name(nullptr)
    {}

    int offset;
    int duration;
    int midi;
    accidental_e accidental;
    int octave;
    const char* step;
    const char* name;
    voice_ptr voice;
    motif_variation_ptr motif;
    position_weak_ptr parent_position;
};

} // namespace dryad