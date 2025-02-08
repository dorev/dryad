#include "motif.h"
#include "score.h"
#include "voice.h"

dryad_motif_note::dryad_motif_note(dryad_note_relative value, dryad_time duration, dryad_time position)
    : value(value)
    , duration(duration)
    , position(position)
{
}

dryad_motif_note* dryad_motif::add_note(dryad_note_relative value, dryad_time duration, dryad_time position)
{
    dryad_motif_note* motif_note = graph->create<dryad_motif_note>(value, duration, position);

    if (motif_note)
        graph->link(this, motif_note);

    update_duration();

    return motif_note;
}

void dryad_motif::update_duration()
{
    dryad_time calculated_duration = 0;
    for_each<dryad_motif_note>([&](dryad_motif_note* note)
        {
            dryad_time note_end = note->position + note->duration;

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
    for_each_breakable<dryad_motif_note>([&](dryad_motif_note* motif_note) -> bool
        {
            return motif_note_owned = (motif_note_to_remove == motif_note);
        });

    if (!motif_note_owned)
        return false;

    // Remove all uncommitted notes instances bound to motif_note_to_remove
    for_each<dryad_note_instance>([&](dryad_note_instance* note)
        {
            if (!note->score_frame->committed)
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

    for_each<dryad_motif_instance>([&](dryad_motif_instance* instance)
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

dryad_error dryad_motif::get_next_instance_time_allowed(dryad_voice* voice, dryad_time& time)
{
    return dryad_error_not_implemented;
}

dryad_time dryad_motif_instance::get_end_time()
{
    if (!motif)
        return dryad_invalid;

    return position + motif->duration;
}
