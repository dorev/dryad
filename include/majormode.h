#pragma once

#include "modegraph.h"
#include "degreenode.h"

namespace dryad
{

class major_mode : public mode_graph
{
public:

    major_mode();

private:

    degree_node _I;
    degree_node _ii;
    degree_node _iii;
    degree_node _IV;
    degree_node _V;
    degree_node _vi;
    degree_node _viid;
};

}