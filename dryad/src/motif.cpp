#include "motif.h"
#include "score.h"
#include "voice.h"

dryad_motif_note::dryad_motif_note(dryad_motif* motif, dryad_note_relative relative_value, dryad_time duration, dryad_time relative_position)
    : relative_value(relative_value)
    , duration(duration)
    , relative_position(relative_position)
{
}

dryad_note_instance::dryad_note_instance(dryad_note_value value, dryad_time duration)
    : value(value)
    , duration(duration)
{
}

dryad_motif_note* dryad_motif::add_note(dryad_note_relative relative_value, dryad_time duration, dryad_time relative_position)
{
    dryad_motif_note* motif_note = graph->create<dryad_motif_note>(this, relative_value, duration, relative_position);

    if (!motif_note)
        return nullptr;

    graph->link(this, motif_note);

    update_duration();

    return motif_note;
}

void dryad_motif::update_duration()
{
    dryad_time calculated_duration = 0;
    for_each_edge<dryad_motif_note>([&](dryad_motif_note* note)
        {
            dryad_time note_end = note->relative_position + note->duration;

            if (note_end > calculated_duration)
                calculated_duration = note_end;
        });

    if (calculated_duration > duration)
        duration = calculated_duration;
}

bool dryad_motif::destroy_note(dryad_motif_note* motif_note_to_remove)
{
    bool motif_note_owned = false;

    // this for loop breaks when returning true
    for_each_edge_breakable<dryad_motif_note>([&](dryad_motif_note* motif_note) -> bool
        {
            return motif_note_owned = (motif_note_to_remove == motif_note);
        });

    if (!motif_note_owned)
        return false;

    // Remove all uncommitted notes instances bound to motif_note_to_remove
    for_each_edge<dryad_note_instance>([&](dryad_note_instance* note)
        {
            dryad_score_frame* note_frame = note->find_first_edge<dryad_score_frame>();

            if (note_frame && !note_frame->committed)
                graph->destroy(note);
            else
                note->remove_edge(motif_note_to_remove);
        });

    return graph->destroy(motif_note_to_remove);
}

dryad_motif_instance* dryad_motif::get_last_instance()
{
    dryad_motif_instance* last_instance = nullptr;
    dryad_time last_instance_end = 0;

    for_each_edge<dryad_motif_instance>([&](dryad_motif_instance* instance)
        {
            dryad_time instance_end = instance->position + duration;
            if (instance_end > last_instance_end)
            {
                last_instance_end = instance_end;
                last_instance = instance;
            }
        });

    return last_instance;
}

dryad_error dryad_motif::get_instances_end_time(dryad_voice* voice, dryad_time& time)
{
    return dryad_error_not_implemented;
}

dryad_motif_instance::dryad_motif_instance(dryad_time position)
    : position(position)
{
    if (position == dryad_invalid)
        DRYAD_ERROR("Motif instance constructed with invalid position.");
}

dryad_time dryad_motif_instance::get_end_time()
{
    dryad_motif* motif = find_first_edge<dryad_motif>();
    if (!motif)
        return dryad_invalid;

    return position + motif->duration;
}
