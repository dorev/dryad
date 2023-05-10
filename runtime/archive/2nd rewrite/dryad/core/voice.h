#pragma once

#include "definitions.h"
#include "monitoring.h"

namespace dryad
{

struct score_t;

struct voice_t : monitor_count<voice_t>
{
    voice_t()
        : octave(5)
        , name("")
    {}

    int octave;
    std::string name;
    score_weak_ptr parent_score;
};

} // namespace dryad