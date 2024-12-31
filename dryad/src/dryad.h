#pragma once

#include "types.h"
#include "graph.h"
#include "flag.h"
#include "constants.h"

using namespace dryad_constants;

DRYAD_DECLARE_FLAG_ENUM(dryad_degree, unsigned)
{
    DRYAD_FLAG(tonic, 1),
    DRYAD_FLAG(supertonic, 2),
    DRYAD_FLAG(mediant, 3),
    DRYAD_FLAG(subdominant, 4),
    DRYAD_FLAG(dominant, 5),
    DRYAD_FLAG(submediant, 6),
    DRYAD_FLAG(leading_tone, 7),
    all = tonic | supertonic | mediant | subdominant | dominant | submediant | leading_tone
};

DRYAD_DECLARE_FLAG_ENUM(dryad_chord_quality, unsigned)
{
    default = 0,
    minor = 1,
    major = 2,
    half_diminished = 3,
    augmented = 4,
    sus2 = 5,
    sus4 = 6,
    diminished = 7,
    DRYAD_FLAG(seventh, 4),
    DRYAD_FLAG(major_seventh, 5),
    DRYAD_FLAG(add6, 6),
    DRYAD_FLAG(add9, 7),
    DRYAD_FLAG(add11, 8),
    DRYAD_FLAG(add13, 9),
    DRYAD_FLAG(flat2, 10),
    DRYAD_FLAG(flat3, 11),
    DRYAD_FLAG(flat4, 12),
    DRYAD_FLAG(flat5, 13),
    DRYAD_FLAG(flat6, 14),
    DRYAD_FLAG(flat7, 15),
    DRYAD_FLAG(flat9, 16),
    DRYAD_FLAG(flat11, 17),
    DRYAD_FLAG(flat13, 18),
    DRYAD_FLAG(sharp2, 19),
    DRYAD_FLAG(sharp3, 20),
    DRYAD_FLAG(sharp4, 21),
    DRYAD_FLAG(sharp5, 22),
    DRYAD_FLAG(sharp6, 23),
    DRYAD_FLAG(sharp7, 24),
    DRYAD_FLAG(sharp9, 25),
    DRYAD_FLAG(sharp11, 26),
    DRYAD_FLAG(sharp13, 27),
    //DRYAD_FLAG(PLACEHOLDER_BIT0, 28),
    //DRYAD_FLAG(PLACEHOLDER_BIT1, 29),
    //DRYAD_FLAG(PLACEHOLDER_BIT2, 30),
    //DRYAD_FLAG(PLACEHOLDER_BIT3, 31),
};

enum class dryad_accidental
{
    natural,
    flat,
    sharp,
};

class dryad_chord : public dryad_node
{
public:
    DRYAD_CLASS_ID(dryad_chord);

    dryad_chord
    (
        dryad_degree degrees = dryad_degree::tonic,
        dryad_chord_quality qualities = dryad_chord_quality::default,
        dryad_accidental accidental = dryad_accidental::natural
    );

    dryad_degree degree;
    dryad_chord_quality qualities;
    dryad_accidental accidental;
};

class dryad_progression_node : public dryad_node
{
public:
    virtual dryad_progression_node* get_next() = 0;
};

class dryad_progression_sequential_switcher : public dryad_progression_node
{
public:
    DRYAD_CLASS_ID(dryad_progression_switcher);

    dryad_vector<dryad_progression_node*> outputs;
    int output_index;
};

class dryad_progression_chord : public dryad_progression_node
{
public:
    DRYAD_CLASS_ID(dryad_progression_chord);

    dryad_chord* chord;
    dryad_note_value root;
    int duration;
};

class dryad_progression : public dryad_node
{
public:
    DRYAD_CLASS_ID(dryad_progression);

    dryad_error set_entry_node(dryad_progression_node* node);
    dryad_error add_node(dryad_progression_node* node);
    dryad_error remove_node(dryad_progression_node* node);


    dryad_progression_node* entry_node;
};

struct dryad_scale_note_offsets
{
    // Defaults to major scale
    dryad_scale_note_offsets
    (
        dryad_note_value tonic = 0,
        dryad_note_value supertonic     = major_second,
        dryad_note_value mediant        = major_third,
        dryad_note_value subdominant    = perfect_fourth,
        dryad_note_value dominant       = perfect_fifth,
        dryad_note_value submediant     = major_sixth,
        dryad_note_value leading_tone   = major_seventh
    )
        : degrees
        {
            tonic,
            supertonic,
            mediant,
            subdominant,
            dominant,
            submediant,
            leading_tone
        }
    {
    }

    dryad_note_value degrees[7];
};

struct dryad_scale_degree_qualities
{
    dryad_scale_degree_qualities
    (
        dryad_chord_quality tonic,
        dryad_chord_quality supertonic,
        dryad_chord_quality mediant,
        dryad_chord_quality subdominant,
        dryad_chord_quality dominant,
        dryad_chord_quality submediant,
        dryad_chord_quality leading_tone
    )
        : degrees
        {
            tonic,
            supertonic,
            mediant,
            subdominant,
            dominant,
            submediant,
            leading_tone
        }
    {
    }

    dryad_chord_quality degrees[7];
};

struct dryad_scale_library
{
    static const inline dryad_scale_note_offsets ionian_offsets = dryad_scale_note_offsets(0, 2, 4, 5, 7, 9, 11);
    static const inline dryad_scale_note_offsets major_offsets = ionian_offsets;
    static const inline dryad_scale_note_offsets dorian_offsets = dryad_scale_note_offsets(0, 2, 3, 5, 7, 9, 10);
    static const inline dryad_scale_note_offsets phrygian_offsets = dryad_scale_note_offsets(0, 1, 3, 5, 7, 8, 10);
    static const inline dryad_scale_note_offsets lydian_offsets = dryad_scale_note_offsets(0, 2, 4, 6, 7, 9, 11);
    static const inline dryad_scale_note_offsets mixolydian_offsets = dryad_scale_note_offsets(0, 2, 4, 5, 7, 9, 10);
    static const inline dryad_scale_note_offsets aeolian_offsets = dryad_scale_note_offsets(0, 2, 3, 5, 7, 8, 10);
    static const inline dryad_scale_note_offsets minor_natural_offsets = aeolian_offsets;
    static const inline dryad_scale_note_offsets locrian_offsets = dryad_scale_note_offsets(0, 1, 3, 5, 6, 8, 10);
    static const inline dryad_scale_note_offsets minor_harmonic_offsets = dryad_scale_note_offsets(0, 2, 3, 5, 7, 8, 11);
    static const inline dryad_scale_note_offsets minor_melodic_offsets = dryad_scale_note_offsets(0, 2, 3, 5, 7, 9, 11);

    static const inline dryad_scale_degree_qualities ionian_degree_qualities = dryad_scale_degree_qualities(
        dryad_chord_quality::major | dryad_chord_quality::major_seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::major | dryad_chord_quality::major_seventh,
        dryad_chord_quality::major | dryad_chord_quality::seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::half_diminished
    );
    
    static const inline dryad_scale_degree_qualities major_degree_qualities = ionian_degree_qualities;

    static const inline dryad_scale_degree_qualities dorian_degree_qualities = dryad_scale_degree_qualities(
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::major | dryad_chord_quality::major_seventh,
        dryad_chord_quality::major | dryad_chord_quality::seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::half_diminished,
        dryad_chord_quality::major | dryad_chord_quality::major_seventh
    );

    static const inline dryad_scale_degree_qualities phrygian_degree_qualities = dryad_scale_degree_qualities(
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::major | dryad_chord_quality::seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::half_diminished,
        dryad_chord_quality::major | dryad_chord_quality::major_seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh
    );

    static const inline dryad_scale_degree_qualities lydian_degree_qualities = dryad_scale_degree_qualities(
        dryad_chord_quality::major | dryad_chord_quality::major_seventh,
        dryad_chord_quality::major | dryad_chord_quality::seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::half_diminished,
        dryad_chord_quality::major | dryad_chord_quality::major_seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::major | dryad_chord_quality::seventh
    );

    static const inline dryad_scale_degree_qualities mixolydian_degree_qualities = dryad_scale_degree_qualities(
        dryad_chord_quality::major | dryad_chord_quality::seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::half_diminished,
        dryad_chord_quality::major | dryad_chord_quality::major_seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::major | dryad_chord_quality::seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh
    );

    static const inline dryad_scale_degree_qualities aeolian_degree_qualities = dryad_scale_degree_qualities(
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::diminished,
        dryad_chord_quality::major | dryad_chord_quality::major_seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::major | dryad_chord_quality::major_seventh,
        dryad_chord_quality::major | dryad_chord_quality::seventh
    );

    static const inline dryad_scale_degree_qualities minor_natural_degree_qualities = aeolian_degree_qualities;

    static const inline dryad_scale_degree_qualities locrian_degree_qualities = dryad_scale_degree_qualities(
        dryad_chord_quality::half_diminished,
        dryad_chord_quality::major | dryad_chord_quality::major_seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::major | dryad_chord_quality::seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh
    );

    static const inline dryad_scale_degree_qualities minor_harmonic_degree_qualities = dryad_scale_degree_qualities(
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::diminished,
        dryad_chord_quality::augmented,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::major | dryad_chord_quality::seventh,
        dryad_chord_quality::major | dryad_chord_quality::major_seventh,
        dryad_chord_quality::diminished
    );

    static const inline dryad_scale_degree_qualities minor_melodic_degree_qualities = dryad_scale_degree_qualities(
        dryad_chord_quality::minor | dryad_chord_quality::major_seventh,
        dryad_chord_quality::minor | dryad_chord_quality::seventh,
        dryad_chord_quality::augmented,
        dryad_chord_quality::major | dryad_chord_quality::major_seventh,
        dryad_chord_quality::major | dryad_chord_quality::seventh,
        dryad_chord_quality::diminished,
        dryad_chord_quality::diminished
    );
};

class dryad_scale : public dryad_node
{
public:
    DRYAD_CLASS_ID(dryad_scale);

    dryad_scale
    (
        dryad_scale_note_offsets note_offsets,
        dryad_scale_degree_qualities degree_qualities
    )
        : note_offsets(note_offsets)
        , dryad_scale_degree_qualities(degree_qualities)
    {
    }

    dryad_scale_note_offsets note_offsets;
    dryad_scale_degree_qualities dryad_scale_degree_qualities;
};

class dryad_voice : public dryad_node
{
public:
    DRYAD_CLASS_ID(dryad_voice);

    int id;
};

class dryad_motif : public dryad_node
{
public:
    DRYAD_CLASS_ID(dryad_motif);


};

class dryad_motif_note : public dryad_node
{
public:
    DRYAD_CLASS_ID(dryad_motif_note);

    dryad_motif* get_motif();

    int position;
    int duration;
    int relative_value;
};

struct dryad_serialized_note
{
    int position;
    int duration;
    int value;
};

struct dryad_serialized_voice
{
    dryad_string name;
    dryad_vector<dryad_serialized_voice> notes;
};

struct dryad_serialized_score
{
    dryad_vector<dryad_serialized_voice> voices;
};

class dryad_score : public dryad_graph
{
public:
    DRYAD_CLASS_ID(dryad_score);

    dryad_error commit(dryad_time duration_step);
    dryad_error dump(dryad_serialized_score& serialized_score);

    dryad_progression* current_progression;
    dryad_scale* current_scale;
    dryad_time committed_end;

};

class dryad_score_frame : public dryad_node
{
public:
    DRYAD_CLASS_ID(dryad_score_frame);

    dryad_score* get_score();

    int position;
};

class dryad_motif_instance : public dryad_node
{
public:
    DRYAD_CLASS_ID(dryad_motif_instance);

    dryad_chord* get_chord();
    dryad_motif* get_motif();
    dryad_score_frame* get_score_frame();
    dryad_voice* get_voice();
};

class dryad_note : public dryad_node
{
public:
    DRYAD_CLASS_ID(dryad_note);

    dryad_motif_note* get_motif_note();
    dryad_motif_instance* get_motif_instance();
    dryad_motif* get_motif();
    dryad_voice* get_voice();
    dryad_score_frame* get_score_frame();

    int duration;
    int value;
};

class dryad_exporter
{
public:
    virtual ~dryad_exporter()
    {
    }

    virtual dryad_error export_score(const dryad_serialized_score& score, dryad_string export_path) = 0;
};
