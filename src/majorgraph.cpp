#include "majorgraph.h"

namespace dryad
{

void major_graph::generate_permutations()
{
    // Hardcoded for test
    _permutations.push_back({ I, IV, V, I });
    _permutations.push_back({ ii, V, I });
    _permutations.push_back({ vi, I });
    _permutations.push_back({ V, I });
}

major_graph::major_graph()
    : _I    (1, MAJOR), I    (&_I)
    , _ii   (2, MINOR), ii   (&_ii)
    , _iii  (3, MINOR), iii  (&_iii)
    , _IV   (4, MAJOR), IV   (&_IV)
    , _V    (5, MAJOR), V    (&_V)
    , _vi   (6, MINOR), vi   (&_vi)
    , _viid (7, DIM)  , viid (&_viid)
{
    _degrees.insert(_degrees.end(), { I, ii, iii, IV, V, vi, viid });

    // Adding basic edges + circle of fifth
    //I    ->add_edge({ ii, iii, IV, V, vi, viid });
    //ii   ->add_edge({ I, IV, V, vi, viid });
    //iii  ->add_edge({ I, V, vi, viid });
    //IV   ->add_edge({ I, ii, V, vi, viid });
    //V    ->add_edge({ I, ii, IV });
    //vi   ->add_edge({ I, ii, iii, IV, V });
    //viid ->add_edge({ I, iii, IV });

    // Basic edges
    I    ->add_edge({ ii, iii, IV, V, vi, viid });
    ii   ->add_edge({ V, viid });
    iii  ->add_edge({ vi });
    IV   ->add_edge({ V, viid });
    V    ->add_edge({ I });
    vi   ->add_edge({ ii, IV });
    viid ->add_edge({ I, iii });

    generate_permutations();
}

void major_graph::print_permutations()
{
    LOG(_permutations.size() << " major permutations");

    int prog_counter = 0;
            
    for (std::vector<degree_node*>& progression : _permutations)
    {
        std::cout << ++prog_counter << " : ";
        for (degree_node* degree : progression)
        {
            std::cout << degree->get_name().c_str() << " ";
        } 
        std::cout << "\n";
    }
}

}