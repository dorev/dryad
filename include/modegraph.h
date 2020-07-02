#include "dryadcommon.h"

namespace dryad
{

class degree_node;

class mode_graph
{
public:

    virtual void print_permutations() final;
    virtual void generate_permutations(size_t max_prog_length = 8) final;

protected:

    std::vector<degree_node*> _degrees;
    std::vector<std::vector<degree_node*>> _permutations;

    mode_graph() {}
    virtual ~mode_graph() {}

};

}