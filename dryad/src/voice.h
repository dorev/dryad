#pragma once

#include "graph.h"

class dryad_motif;
class dryad_motif_instance;

class dryad_voice : public dryad_node
{
public:
    DRYAD_NODE_CLASS_ID(dryad_voice);

    struct compare_by_id
    {
        bool operator()(const dryad_voice* a, const dryad_voice* b) const
        {
            return a->id < b->id;
        }
    };

    dryad_voice(int id, dryad_string name);
    dryad_error add_motif(dryad_motif* motif);
    dryad_error remove_motif(dryad_motif* motif);
    dryad_motif_instance* get_last_motif_instance();
    dryad_error generate_until(dryad_time position_target);
    dryad_error add_motif_instance(dryad_motif* motif, dryad_motif_instance*& instance);

    int id;
    dryad_string name;
    dryad_vector<dryad_motif*> motifs;
};
