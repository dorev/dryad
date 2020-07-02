#include "dryadcommon.h"

namespace dryad
{

class degree_node;

class mode_graph
{
public:

    virtual void print_permutations() final;

protected:

    std::vector<degree_node*> _degrees;
    std::vector<std::vector<degree_node*>> _permutations;

    mode_graph() {}
    virtual ~mode_graph() {}

    virtual void generate_permutations(size_t max_node_visit = 1, size_t max_prog_length = 8) final;
};

}