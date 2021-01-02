#pragma once

#include "includes.h"

namespace dryad
{
namespace model
{

struct position;
struct voice;
struct motif;

struct note
{
    // Members
    int offset;
    int duration;
    int alteration;
    int octave;
    const char* step;

    // References
    std::shared_ptr<voice> associated_voice;
    std::shared_ptr<motif> associated_motif;
    std::shared_ptr<position> parent_position;
};

} // namespace model
} // namespace dryad