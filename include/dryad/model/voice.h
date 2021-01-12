#pragma once

#include "dryad/model/definitions.h"

namespace dryad
{

struct score_t;

struct voice_t
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