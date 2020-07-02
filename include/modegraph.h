#include "dryadcommon.h"

namespace dryad
{

class degree_node;

class mode_graph
{
public:

    virtual void print_permutations() final;

protected:

    std::vector<const degree_node*> _degrees;
    std::vector<std::vector<const degree_node*>> _permutations;

    mode_graph() {}
    virtual ~mode_graph() {}

    virtual void generate_permutations() final;
};

}