#include "majormode.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

major_mode::major_mode()
    : _I    (1, triad::major)
    , _ii   (2, triad::minor)
    , _iii  (3, triad::minor)
    , _IV   (4, triad::major)
    , _V    (5, triad::major)
    , _vi   (6, triad::minor)
    , _viid (7, triad::dim)
{
    degree_node* I    = &_I;
    degree_node* ii   = &_ii;
    degree_node* iii  = &_iii;
    degree_node* IV   = &_IV;
    degree_node* V    = &_V;
    degree_node* vi   = &_vi;
    degree_node* viid = &_viid;

    _degrees = { I, ii, iii, IV, V, vi, viid };

    // Adding basic edges + circle of fifth
    //I    ->add_edge({ ii, iii, IV, V, vi, viid });
    //ii   ->add_edge({ I, IV, V, vi, viid });
    //iii  ->add_edge({ I, V, vi, viid });
    //IV   ->add_edge({ I, ii, V, vi, viid });
    //V    ->add_edge({ I, ii, IV });
    //vi   ->add_edge({ I, ii, iii, IV, V });
    //viid ->add_edge({ I, iii, IV });

    // Basic edges
    _I.add_edge({ ii, iii, IV, V, vi, viid })
        .mark_as_entry()
        .mark_as_exit()
        .set_max_visit(2);

    _ii.add_edge({ V, viid });

    _iii.add_edge({ vi })
        .mark_as_entry();

    _IV.add_edge({ V, viid })
        .mark_as_entry()
        .mark_as_exit()
        .set_max_visit(2);

    _V.add_edge({ I })
        .mark_as_entry()
        .mark_as_exit()
        .set_max_visit(2);

    _vi.add_edge({ ii, IV })
        .mark_as_entry()
        .mark_as_exit()
        .set_max_visit(2);

    _viid.add_edge({ I, iii })
        .mark_as_exit();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}