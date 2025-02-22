#include "voice.h"
#include "score.h"
#include "motif.h"

dryad_voice::dryad_voice(int id, dryad_string name)
    : id(id)
    , name(name)
{
}

dryad_error dryad_voice::add_motif(dryad_motif* motif)
{
    if (!graph->contains(motif))
        return dryad_error_node_not_in_graph;

    if (std::find(motifs.begin(), motifs.end(), motif) != motifs.end())
        return dryad_error_already_exists;

    graph->link(this, motif);

    // Keep a cached pointer to avoid cycling through all edges
    motifs.push_back(motif);

    return dryad_error_success;
}

dryad_error dryad_voice::remove_motif(dryad_motif* motif)
{
    return dryad_error_not_implemented;
}

dryad_motif_instance* dryad_voice::get_last_motif_instance()
{
    // TODO : implement
    return nullptr;
}

dryad_error dryad_voice::generate_until(dryad_time position_target)
{
    dryad_score* score = static_cast<dryad_score*>(graph);
    if (!score)
        return dryad_error_node_not_in_graph;

    for (dryad_motif* motif : motifs)
    {
        dryad_motif_instance* motif_instance = get_last_motif_instance();
        dryad_time next_motif_time = 0;

        if (!motif_instance)
        {
            dryad_error error = motif->get_instances_end_time(this, next_motif_time);
            if (error)
                return error;
        }
        else
        {
            next_motif_time = motif_instance->get_end_time();
            if (next_motif_time == dryad_invalid)
                return dryad_error_invalid_instance;
        }

        if (next_motif_time >= position_target)
        {
            // Committing the requested duration doesn't required additional
            // instances generation for this motif
            continue;
        }
        else
        {
            while (next_motif_time < position_target)
            {
                dryad_error error = score->add_motif_instance(this, motif, next_motif_time, motif_instance);
                if (error)
                    return error;

                next_motif_time = motif_instance->get_end_time();
            }
        }
    }

    return dryad_error_success;
}
