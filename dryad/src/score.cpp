#include "score.h"
#include "motif.h"
#include "scale.h"
#include "progression.h"
#include "chord.h"

dryad_score_frame::dryad_score_frame(dryad_time relative_position)
    : relative_position(relative_position)
    , committed(false)
{
}

dryad_score* dryad_score_frame::get_score()
{
    return static_cast<dryad_score*>(graph);
}

dryad_note_value dryad_score_frame::get_current_root()
{
    return get_score()->current_root;
}

dryad_scale* dryad_score_frame::get_current_scale()
{
    return get_score()->current_scale;
}

dryad_chord dryad_score_frame::get_current_chord()
{
    return get_score()->current_progression->current_progression_chord->chord;
}

dryad_error dryad_score_frame::add_motif_note(dryad_motif_note* motif_note)
{
    if (!motif_note)
        return dryad_error_invalid_motif_note;

    dryad_motif* motif = motif_note->motif;
    if (!motif)
        return dryad_error_invalid_motif;

    dryad_note_value note_anchor = get_current_root();

    // we have to find the current scale to know the root
    // then we check the harmonic anchor of the motif
    // if it's the scale...
    // if it's the chord...
    // TODO: at some point we will also have to take the voice range in consideration
    if (motif->harmonic_anchor == dryad_harmonic_anchor::scale)
    {
        dryad_scale* scale = get_current_scale();

        dryad_note_relative note_octave = motif_note->relative_value / 12;
        dryad_note_relative note_offset = scale->note_offsets.degrees[motif_note->relative_value % static_cast<int>(dryad_degree::limit)];
        dryad_note_value note = dryad_constants::notes[C][4] + 12 * note_octave + note_offset;

        dryad_note_instance* note_instance = graph->create<dryad_note_instance>(this, motif_note, note, motif_note->duration);
        graph->link(note_instance, this);
        graph->link(note_instance, motif_note);

        // CATCH UP HERE
    }

    return dryad_error_not_implemented;
}

dryad_score::dryad_score()
    : current_root(dryad_constants::C)
    , current_progression(nullptr)
    , current_scale(nullptr)
{
}

dryad_voice* dryad_score::add_voice(int id, dryad_string name)
{
    dryad_voice* voice = create<dryad_voice>(id, name);

    if (voice)
        voices.insert(voice);

    return voice;
}

dryad_error dryad_score::add_motif_instance(dryad_voice* voice, dryad_motif* motif, dryad_time position, dryad_motif_instance*& instance)
{
    if (!voice)
        return dryad_error_invalid_voice;

    if (!motif)
        return dryad_error_invalid_motif;

    if (position == dryad_invalid)
        return dryad_error_invalid_position;

    instance = create<dryad_motif_instance>(voice, motif, position);
    if (!instance)
        return dryad_error_invalid_instance;

    // TODO: we have to identify of there is a rythmic anchor to respect for the motif!!

    // Add each note of the motif in already existing or new frames
    for (dryad_motif_note* note : motif->notes)
    {
        dryad_score_frame* frame = get_or_create_frame(position + note->relative_position);
        if (!frame)
            return dryad_error_invalid_frame;

        dryad_error error = frame->add_motif_note(note);
        if (error)
            return error;
    }

    return dryad_error_success;
}

dryad_error dryad_score::commit(dryad_time duration_to_commit)
{
    dryad_score_frame* frame = find_last_committed_frame();

    if (!frame)
        frame = get_or_create_frame(0);

    dryad_time relative_position = frame->relative_position + duration_to_commit;

    // For every motif of each voice, generate instances until the total committed
    // duration is reached
    for (dryad_voice* voice : voices)
        voice->generate_until(relative_position);

    // for all voices
        // check if motifs notes are framed until the final committed duration
            // print them if they are not
            // how do we 'print' a motif?
                // consider the current scale and progression chords
                // for each motif note, until the notes printed extend beyond the final committed duration
                    // create a score_frames if it does not exist yet
                    // add all necessary links to the frame (motif_instance, motif_notes, progression_chord_instance, scale)
                    // have score_frame generate the note_instances
        // commit all frames within the committed duration

    return dryad_error_not_implemented;
}

dryad_error dryad_score::dump(dryad_serialized_score& serialized_score)
{
    return dryad_error_not_implemented;
}

dryad_score_frame* dryad_score::get_or_create_frame(dryad_time relative_position)
{
    dryad_score_frame* frame = find_frame_at_position(relative_position);

    if (frame)
        return frame;

    frame = create<dryad_score_frame>(relative_position);

    if (!frame)
    {
        DRYAD_ERROR("Unable to create a new frame at position %d", relative_position)
    }
    else
    {
        frames.insert(frame);
    }

    return frame;
}

dryad_score_frame* dryad_score::find_frame_at_position(dryad_time relative_position)
{
    // Creating a dummy frame with the searched position to leverage std::find
    dryad_score_frame dummy_frame(relative_position);

    auto it = frames.find(&dummy_frame);

    if (it == frames.end())
        return nullptr;

    return *it;
}

dryad_score_frame* dryad_score::find_last_committed_frame()
{
    dryad_score_frame* frame = nullptr;

    for (auto it = frames.rbegin(); it != frames.rend(); ++it)
    {
        if ((*it)->committed)
        {
            frame = *it;
            break;
        }
    }

    return frame;
}
