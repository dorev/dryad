#pragma once

#include "types.h"

struct dryad_serialized_note
{
    dryad_time position;
    dryad_time duration;
    dryad_note_value value;
};

struct dryad_serialized_voice
{
    dryad_string name;
    int id;
    dryad_vector<dryad_serialized_voice> notes;
};

struct dryad_serialized_score
{
    dryad_vector<dryad_serialized_voice> voices;
};

class dryad_exporter
{
public:
    virtual ~dryad_exporter()
    {
    }

    virtual dryad_error export_score(const dryad_serialized_score& score, dryad_string export_path) = 0;
};
