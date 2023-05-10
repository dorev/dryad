#include "pitch.h"

namespace dryad
{

pitch_t::pitch_t(int pitch, accidental accidental_type)
    : _step(0)
    , _alter(0)
    , _octave(0)
{
    if (pitch < 0 || pitch > 120)
    {
        CRASH("Pitch value " << pitch << " is invalid. It should be with 0 and 120.");
    }

    integer_to_step_and_alteration(pitch, _step, _alter, accidental_type);

    _octave = pitch / 12;
}

}