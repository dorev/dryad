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

    dryad_motif_note(dryad_motif* motif, dryad_note_relative relative_value, dryad_time duration, dryad_time relative_position);

    dryad_note_relative relative_value;
    dryad_time duration;
    dryad_time relative_position;
};


class dryad_note_instance : public dryad_node
{
public:
    DRYAD_NODE_CLASS_ID(dryad_note_instance);

    dryad_note_instance(dryad_note_value value, dryad_time duration);

    dryad_note_value value;
    dryad_time duration;
};


class dryad_motif : public dryad_node
{
public:
    DRYAD_NODE_CLASS_ID(dryad_motif);

    dryad_harmonic_anchor harmonic_anchor;
    dryad_rhythmic_anchor rhythmic_anchor;
    dryad_note_offset_type note_offset_type;
    dryad_time duration;

    dryad_motif_note* add_note(dryad_note_relative relative_value, dryad_time duration, dryad_time relative_position);

    // Evaluates the duration of the motif by checking the end time
    // of each note. The duration can be specified beyond that time
    // so the silence at the end of the motif is also included
    void update_duration();

    // A motif_note should never be reused once it's destroyed
    bool destroy_note(dryad_motif_note* motif_note_to_remove);

    dryad_motif_instance* get_last_instance();
    dryad_error get_instances_end_time(dryad_voice* voice, dryad_time& time);

};

class dryad_motif_instance : public dryad_node
{
public:
    DRYAD_NODE_CLASS_ID(dryad_motif_instance);

    dryad_motif_instance(dryad_time position);

    dryad_time get_end_time();

    dryad_time position;
};