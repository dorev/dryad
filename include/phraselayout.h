#include "barlayout.h"

namespace dryad
{

class phrase_layout
{
public:

    phrase_layout(std::vector<degree_node*> degrees, size_t bar_count = 4);

    void generate_permutations();

private:

    std::vector<std::vector<bar_layout>> _bars;

};

}