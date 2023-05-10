#pragma once

#include "dryad/types.h"
#include "dryad/model/scaleconfig.h"

namespace Dryad::Model
{

class Scale
{
    ScaleConfig _config;
    String _name;
    /*
    scale_ptr create_major_scale();
    void add_degree(scale_ptr scale, degree_ptr degree);
    void add_degrees(scale_ptr scale, std::initializer_list<degree_ptr> degrees);
    const std::vector<int>& get_chord_interval(degree_ptr degree);
    */
};

} // namespace Dryad::Model