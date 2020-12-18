#pragma once

#include "dryadutils.h"
#include "dryadinfo.h"
#include "modegraph.h"
#include "degreenode.h"

namespace dryad
{

class major_mode : public mode_graph, dryad_info<major_mode>
{
public:

    major_mode();

private:

    degree_t _I;
    degree_t _ii;
    degree_t _iii;
    degree_t _IV;
    degree_t _V;
    degree_t _vi;
    degree_t _viid;
};

}