#pragma once

#include "dryad/model.h"

namespace Dryad::Export
{
    using namespace Model;

    void* ToMidi(const Score& score);
    void* ToMusicXml(const Score& score);

} // namespace Dryad::Export