#include "dryadcommon.h"

namespace dryad
{

class degree_node;
    
class mode_graph
{
    protected:
    std::vector<degree_node*> _degrees;
    virtual void generate_permutations() = 0;
    virtual void print_permutations() = 0;
};

}