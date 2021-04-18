#pragma once

#include "dryad/types.h"
#include "dryad/model/traits.h"
#include "dryad/model/position.h"

namespace Dryad::Model
{

class Measure : public Listed<Measure>
{
    Vector<Position> _positions;
    /*
    int get_total_voice_duration(measure_ptr measure, voice_ptr voice);
    position_ptr get_position_at_time(measure_ptr measure, int time);
    position_ptr insert_position_at_time(measure_ptr measure, int time);
    */
};

} // namespace Dryad::Model