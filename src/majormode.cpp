#include "majormode.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

major_mode::major_mode()
    : _I    (1, MAJOR), I    (&_I)
    , _ii   (2, MINOR), ii   (&_ii)
    , _iii  (3, MINOR), iii  (&_iii)
    , _IV   (4, MAJOR), IV   (&_IV)
    , _V    (5, MAJOR), V    (&_V)
    , _vi   (6, MINOR), vi   (&_vi)
    , _viid (7, DIM)  , viid (&_viid)
{
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
        .mark_as_exit();

    _ii.add_edge({ V, viid });

    _iii.add_edge({ vi })
        .mark_as_entry();

    _IV.add_edge({ V, viid })
        .mark_as_entry()
        .mark_as_exit();

    _V.add_edge({ I })
        .mark_as_entry()
        .mark_as_exit();

    _vi.add_edge({ ii, IV })
        .mark_as_entry()
        .mark_as_exit();

    _viid.add_edge({ I, iii })
        .mark_as_exit();

    generate_permutations(2, 4);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}