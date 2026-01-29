#pragma once

#include "chord.h"
#include "types.h"

namespace Dryad
{

    Error getChordOffsetsFromRoot(const Chord& chord, const Scale* scale, Vector<NoteRelative>& offsets);

} // namespace Dryad
