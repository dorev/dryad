#pragma once

#include "graph.h"
#include "voice.h"
#include "chord.h"

class dryad_motif_note;
class dryad_scale;
class dryad_score;

class dryad_score_frame : public dryad_node
{
public:
    DRYAD_NODE_CLASS_ID(dryad_score_frame);

    dryad_score_frame(dryad_time relative_position = 0);

    // Comparison helper to automate the ordering in a set
    struct compare_by_position
    {
        bool operator()(const dryad_score_frame* a, const dryad_score_frame* b) const
        {
            return a->relative_position < b->relative_position;
        }
    };

    dryad_score* get_score();
    dryad_note_value get_current_root();
    dryad_scale* get_current_scale();
    dryad_chord get_current_chord();
    dryad_error add_motif_note(dryad_motif_note* motif_note);

    dryad_time relative_position;
    bool committed;
};

class dryad_progression;
class dryad_scale;
struct dryad_serialized_score;

class dryad_score : public dryad_graph
{
public:
    DRYAD_NODE_CLASS_ID(dryad_score);

    dryad_score();

    dryad_voice* add_voice(int id, dryad_string name);
    dryad_error add_motif_instance(dryad_voice* voice, dryad_motif* motif, dryad_time position, dryad_motif_instance*& instance);

    // Calling this will set in stone the notes within the specified duration,
    // effectively appending 'real' notes to the score.
    //
    // The steps of this process are:
    //
    // - Check that all motifs in all voices are generated at least until the end of
    //   the committed duration (include the duration appended by this call)
    //
    // - Each newly committed frame will then evaluate its notes based on the associated
    //   motif parameters and considering the scale and progression chord of the frame
    dryad_error commit(dryad_time duration_to_commit);

    dryad_error dump(dryad_serialized_score& serialized_score);
    dryad_score_frame* get_or_create_frame(dryad_time relative_position);
    dryad_score_frame* find_frame_at_position(dryad_time relative_position);
    dryad_score_frame* find_last_committed_frame();

    dryad_note_value current_root;
    dryad_progression* current_progression;
    dryad_scale* current_scale;
    dryad_set<dryad_voice*, dryad_voice::compare_by_id> cached_voices;
    dryad_set<dryad_score_frame*, dryad_score_frame::compare_by_position> cached_frames;
};
