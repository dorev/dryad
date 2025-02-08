#pragma once

#include "types.h"
#include "graph.h"
#include "chord.h"
#include "constants.h"
using namespace dryad_constants;

class dryad_motif;
class dryad_voice;
class dryad_scale;

class dryad_progression_node : public dryad_node
{
public:

    dryad_progression_node()
        : next(nullptr)
    {
    }

    dryad_progression_node* next;
};

class dryad_progression : public dryad_node
{
public:
    DRYAD_NODE_CLASS_ID(dryad_progression);

    dryad_progression_node* entry_node;
    dryad_vector<dryad_progression_node*> nodes;
};

class dryad_progression_switch_sequence : public dryad_progression_node
{
public:
    DRYAD_NODE_CLASS_ID(dryad_progression_switch_sequence);

    dryad_vector<dryad_progression_node*> outputs;
    int output_index;
};

struct dryad_motif_change
{
    dryad_motif* old_motif;
    dryad_motif* new_motif;
};

struct dryad_voice_change
{
    dryad_voice* old_voice;
    dryad_voice* new_voice;
};

class dryad_progression_score_event : public dryad_progression_node
{
public:
    DRYAD_NODE_CLASS_ID(dryad_progression_score_event);

    dryad_progression* progression_change;
    dryad_scale* scale_change;
    dryad_vector<dryad_motif_change> motif_changes;
    dryad_vector<dryad_voice_change> voice_changes;
    bool score_end;
};

class dryad_progression_chord : public dryad_progression_node
{
public:
    DRYAD_NODE_CLASS_ID(dryad_progression_chord);

    dryad_progression_chord(dryad_chord chord, dryad_time duration)
        : chord(chord)
        , duration(duration)
    {
    }

    dryad_chord chord;
    dryad_time duration;
};

class dryad_progression_chord_instance : public dryad_node
{
public:
    DRYAD_NODE_CLASS_ID(dryad_progression_chord_instance);

    dryad_progression_chord* progression_chord;
    dryad_note_value root;
    dryad_time position;
};
