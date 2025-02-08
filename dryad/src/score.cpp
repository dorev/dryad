#include "score.h"

dryad_score_frame::dryad_score_frame(dryad_time position)
    : position(position)
    , committed(false)
{
}

dryad_score::dryad_score()
    : current_progression(nullptr)
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

dryad_error dryad_score::commit(dryad_time duration_to_commit)
{
    dryad_score_frame* frame = find_last_committed_frame();

    if (!frame)
        frame = get_or_create_frame(0);

    dryad_time position = frame->position + duration_to_commit;

    // For every motif of each voice, generate instances until the total committed
    // duration is reached
    for (dryad_voice* voice : voices)
    {
        voice->generate_until(position);
    }

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

dryad_score_frame* dryad_score::get_or_create_frame(dryad_time position)
{
    dryad_score_frame* frame = find_frame_at_position(position);

    if (frame)
        return frame;

    frame = create<dryad_score_frame>(position);

    frames.insert(frame);

    return frame;
}

dryad_score_frame* dryad_score::find_frame_at_position(dryad_time position)
{
    // Creating a dummy frame with the searched position to leverage std::find
    dryad_score_frame dummy_frame(position);

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
