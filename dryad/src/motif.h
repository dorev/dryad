#pragma once

#include "graph.h"
#include "constants.h"

enum class dryad_harmonic_anchor
{
    scale,
    chord
};

enum class dryad_rhythmic_anchor
{
    chord_beginning,
    strong_beat,
    weak_beat,
    any_beat
};

enum class dryad_note_offset_type
{
    diatonic,
    chromatic
};

class dryad_motif;
class dryad_motif_instance;
class dryad_score_frame;
class dryad_voice;

class dryad_motif_note : public dryad_node
{
public:
    DRYAD_NODE_CLASS_ID(dryad_motif_note);

    dryad_motif_note(dryad_note_relative value = 0, dryad_time duration = dryad_constants::quarter, dryad_time position = 0);

    dryad_note_relative value;
    dryad_time duration;
    dryad_time position;
};


class dryad_note_instance : public dryad_node
{
public:
    DRYAD_NODE_CLASS_ID(dryad_note_instance);

    dryad_time duration;
    dryad_note_value value;
    dryad_score_frame* score_frame;
    const dryad_motif_note* motif_note;
};


class dryad_motif : public dryad_node
{
public:
    DRYAD_NODE_CLASS_ID(dryad_motif);

    dryad_harmonic_anchor harmonic_anchor;
    dryad_rhythmic_anchor rhythmic_anchor;
    dryad_note_offset_type note_offset_type;
    dryad_time duration;

    dryad_motif_note* add_note(dryad_note_relative value, dryad_time duration, dryad_time position);

    // Evaluates the duration of the motif by checking the end time
    // of each note. The duration can be specified beyond that time
    // so the silence at the end of the motif is also included
    void update_duration();

    // A motif_note should never be reused once it's destroyed
    bool destroy_note(dryad_motif_note* motif_note_to_remove);

    dryad_motif_instance* get_last_instance();
    dryad_error get_next_instance_time_allowed(dryad_voice* voice, dryad_time& time);

};

class dryad_motif_instance : public dryad_node
{
public:
    DRYAD_NODE_CLASS_ID(dryad_motif_instance);

    dryad_time get_end_time();

    dryad_time position;
    const dryad_motif* motif;
};