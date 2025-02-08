#pragma once

#include "types.h"
#include "graph.h"
#include "flag.h"
#include "constants.h"

using namespace dryad_constants;

class dryad_motif;
class dryad_motif_instance;
class dryad_voice;
class dryad_score;
class dryad_scale;
class dryad_progression;

DRYAD_DECLARE_FLAG_ENUM(dryad_degree, unsigned)
{
    invalid = 0,
    DRYAD_FLAG(tonic, 1),
    root = tonic,
    DRYAD_FLAG(supertonic, 2),
    second = supertonic,
    DRYAD_FLAG(mediant, 3),
    third = mediant,
    DRYAD_FLAG(subdominant, 4),
    fourth = subdominant,
    DRYAD_FLAG(dominant, 5),
    fifth = dominant,
    DRYAD_FLAG(submediant, 6),
    sixth = submediant,
    DRYAD_FLAG(leading_tone, 7),
    seventh = leading_tone
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
    DRYAD_FLAG(seventh_minor, 4),
    DRYAD_FLAG(seventh_major, 5),
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

struct dryad_chord
{
    dryad_chord
    (
        dryad_degree degree,
        dryad_chord_quality qualities = dryad_chord_quality::default,
        dryad_accidental accidental = dryad_accidental::natural
    )
        : degree(degree)
        , qualities(qualities)
        , accidental(accidental)
    {
    }

    dryad_degree degree;
    dryad_chord_quality qualities;
    dryad_accidental accidental;
};

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
    DRYAD_CLASS_ID(dryad_progression);

    dryad_progression_node* entry_node;
    dryad_vector<dryad_progression_node*> nodes;
};

class dryad_progression_switch_sequence : public dryad_progression_node
{
public:
    DRYAD_CLASS_ID(dryad_progression_switch_sequence);

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
    DRYAD_CLASS_ID(dryad_progression_score_event);

    dryad_progression* progression_change;
    dryad_scale* scale_change;
    dryad_vector<dryad_motif_change> motif_changes;
    dryad_vector<dryad_voice_change> voice_changes;
    bool score_end;
};

class dryad_progression_chord : public dryad_progression_node
{
public:
    DRYAD_CLASS_ID(dryad_progression_chord);

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
    DRYAD_CLASS_ID(dryad_progression_chord_instance);

    dryad_progression_chord* progression_chord;
    dryad_note_value root;
    dryad_time position;
};

struct dryad_scale_note_offsets
{
    dryad_scale_note_offsets
    (
        dryad_note_value tonic,
        dryad_note_value supertonic,
        dryad_note_value mediant,
        dryad_note_value subdominant,
        dryad_note_value dominant,
        dryad_note_value submediant,
        dryad_note_value leading_tone
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

class dryad_scale : public dryad_node
{
public:
    DRYAD_CLASS_ID(dryad_scale);

    dryad_scale
    (
        dryad_scale_note_offsets scale_note_offsets,
        dryad_scale_degree_qualities degree_qualities
    )
        : note_offsets(scale_note_offsets)
        , degree_qualities(degree_qualities)
    {
    }

    dryad_scale(const dryad_scale& other)
        : note_offsets(other.note_offsets)
        , degree_qualities(other.degree_qualities)
    {
    }

    dryad_scale& operator=(const dryad_scale& other)
    {
        if (&other != this)
        {
            note_offsets = other.note_offsets;
            degree_qualities = other.degree_qualities;
        }

        return *this;
    }

    dryad_scale_note_offsets note_offsets;
    dryad_scale_degree_qualities degree_qualities;
};

struct dryad_scale_library
{
    inline static const dryad_scale_note_offsets ionian_offsets{0, 2, 4, 5, 7, 9, 11};
    inline static const dryad_scale_note_offsets major_offsets = ionian_offsets;
    inline static const dryad_scale_note_offsets dorian_offsets{0, 2, 3, 5, 7, 9, 10};
    inline static const dryad_scale_note_offsets phrygian_offsets{0, 1, 3, 5, 7, 8, 10};
    inline static const dryad_scale_note_offsets lydian_offsets{0, 2, 4, 6, 7, 9, 11};
    inline static const dryad_scale_note_offsets mixolydian_offsets{0, 2, 4, 5, 7, 9, 10};
    inline static const dryad_scale_note_offsets aeolian_offsets{0, 2, 3, 5, 7, 8, 10};
    inline static const dryad_scale_note_offsets minor_natural_offsets = aeolian_offsets;
    inline static const dryad_scale_note_offsets locrian_offsets{0, 1, 3, 5, 6, 8, 10};
    inline static const dryad_scale_note_offsets minor_harmonic_offsets{0, 2, 3, 5, 7, 8, 11};
    inline static const dryad_scale_note_offsets minor_melodic_offsets{0, 2, 3, 5, 7, 9, 11};

    inline static const dryad_scale_degree_qualities ionian_degree_qualities
    {
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::half_diminished
    };
    
    inline static const dryad_scale_degree_qualities major_degree_qualities = ionian_degree_qualities;

    inline static const dryad_scale_degree_qualities dorian_degree_qualities
    {
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::half_diminished,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major
    };

    inline static const dryad_scale_degree_qualities phrygian_degree_qualities
    {
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::half_diminished,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor
    };

    inline static const dryad_scale_degree_qualities lydian_degree_qualities
    {
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::half_diminished,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor
    };

    inline static const dryad_scale_degree_qualities mixolydian_degree_qualitie
    {
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::half_diminished,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor
    };

    inline static const dryad_scale_degree_qualities aeolian_degree_qualities
    {
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::diminished,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor
    };

    inline static const dryad_scale_degree_qualities minor_natural_degree_qualities = aeolian_degree_qualities;

    inline static const dryad_scale_degree_qualities locrian_degree_qualities
    {
        dryad_chord_quality::half_diminished,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor
    };

    inline static const dryad_scale_degree_qualities minor_harmonic_degree_qualities
    {
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::diminished,
        dryad_chord_quality::augmented,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::diminished
    };

    inline static const dryad_scale_degree_qualities minor_melodic_degree_qualities
    {
        dryad_chord_quality::minor | dryad_chord_quality::seventh_major,
        dryad_chord_quality::minor | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::augmented,
        dryad_chord_quality::major | dryad_chord_quality::seventh_major,
        dryad_chord_quality::major | dryad_chord_quality::seventh_minor,
        dryad_chord_quality::diminished,
        dryad_chord_quality::diminished
    };

    inline static const dryad_scale ionian_scale{ionian_offsets, ionian_degree_qualities};
    inline static const dryad_scale major_scale{ionian_scale};
    inline static const dryad_scale dorian_scale{dorian_offsets, dorian_degree_qualities};
    inline static const dryad_scale phrygian_scale{phrygian_offsets, phrygian_degree_qualities};
    inline static const dryad_scale lydian_scale{lydian_offsets, lydian_degree_qualities};
    inline static const dryad_scale mixolydian_scale{mixolydian_offsets, mixolydian_degree_qualitie};
    inline static const dryad_scale aeolian_scale{aeolian_offsets, aeolian_degree_qualities};
    inline static const dryad_scale minor_natural_scale{aeolian_scale};
    inline static const dryad_scale locrian_scale{locrian_offsets, locrian_degree_qualities};
    inline static const dryad_scale minor_harmonic_scale{minor_harmonic_offsets, minor_harmonic_degree_qualities};
    inline static const dryad_scale minor_melodic_scale{minor_melodic_offsets, minor_melodic_degree_qualities};
};

class dryad_voice : public dryad_node
{
public:
    DRYAD_CLASS_ID(dryad_voice);

    struct compare_by_id
    {
        bool operator()(const dryad_voice* a, const dryad_voice* b) const
        {
            return a->id < b->id;
        }
    };

    dryad_voice(int id, dryad_string name)
        : id(id)
        , name(name)
    {
    }

    dryad_error add_motif(dryad_motif* motif)
    {
        if (!graph->contains(motif))
            return dryad_error_node_not_in_graph;

        if (std::find(motifs.begin(), motifs.end(), motif) != motifs.end())
            return dryad_error_already_exists;

        graph->link(this, motif);

        // Keep a cached pointer to avoid cycling through all edges
        motifs.push_back(motif);

        return dryad_error_success;
    }

    dryad_error remove_motif(dryad_motif* motif)
    {
        return dryad_error_not_implemented;
    }

    dryad_motif_instance* get_last_motif_instance()
    {

    }

    dryad_error generate_until(dryad_time position_target)
    {
        dryad_score* score = static_cast<dryad_score*>(graph);
        if (!score)
            return dryad_error_node_not_in_graph;

        for (dryad_motif* motif : motifs)
        {
            dryad_motif_instance* motif_instance = get_last_motif_instance();
            dryad_time next_instance_time = 0;

            if (!motif_instance)
                next_instance_time = motif->get_next_instance_time_allowed();
            else
                next_instance_time = motif_instance->get_end_time();

            if (next_instance_time >= position_target)
            {
                // Committing the requested duration doesn't required additional
                // instances generation for this motif
                continue;
            }
            else
            {
                while (next_instance_time < position_target)
                {
                    dryad_score_frame* frame = score->get_or_create_frame(next_instance_time);
                    if (!frame)
                        return dryad_error_invalid_frame;

                    dryad_error error = frame->append_motif_instance(this, motif, next_instance_time, motif_instance);
                    if (error)
                        return error;

                    next_instance_time = motif_instance->get_end_time();
                }
            }
        }

        return dryad_error_success;
    }

    int id;
    dryad_string name;
    dryad_vector<dryad_motif*> motifs;
};

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

class dryad_motif_note : public dryad_node
{
public:
    DRYAD_CLASS_ID(dryad_motif_note);

    dryad_motif_note(dryad_note_relative value = 0, dryad_time duration = quarter, dryad_time position = 0)
        : value(value)
        , duration(duration)
        , position(position)
    {
    }

    dryad_note_relative value;
    dryad_time duration;
    dryad_time position;
};

class dryad_score_frame : public dryad_node
{
public:
    DRYAD_CLASS_ID(dryad_score_frame);

    dryad_score_frame(dryad_time position = 0)
        : position(position)
        , committed(false)
    {
    }

    // Comparison helper to automate the ordering in a set
    struct compare_by_position
    {
        bool operator()(const dryad_score_frame* a, const dryad_score_frame* b) const
        {
            return a->position < b->position;
        }
    };

    dryad_time position;
    bool committed;
};

class dryad_note_instance : public dryad_node
{
public:
    DRYAD_CLASS_ID(dryad_note_instance);

    dryad_time duration;
    dryad_note_value value;
    dryad_score_frame* score_frame;
    const dryad_motif_note* motif_note;
};

class dryad_motif_instance : public dryad_node
{
public:
    DRYAD_CLASS_ID(dryad_motif_instance);

    dryad_time position;
    const dryad_motif* motif;
};

class dryad_motif : public dryad_node
{
public:
    DRYAD_CLASS_ID(dryad_motif);

    dryad_harmonic_anchor harmonic_anchor;
    dryad_rhythmic_anchor rhythmic_anchor;
    dryad_note_offset_type note_offset_type;
    dryad_time duration;

    dryad_motif_note* add_note(dryad_note_relative value, dryad_time duration, dryad_time position)
    {
        dryad_motif_note* motif_note = graph->create<dryad_motif_note>(value, duration, position);

        if (motif_note)
            graph->link(this, motif_note);

        update_duration();

        return motif_note;
    }

    // void update_note(dryad_motif_note* motif_note);

    // Evaluates the duration of the motif by checking the end time
    // of each note. The duration can be specified beyond that time
    // so the silence at the end of the motif is also included
    void update_duration()
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

    // A motif_note should never be reused once it's destroyed
    bool destroy_note(dryad_motif_note* motif_note_to_remove)
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

    dryad_motif_instance* get_last_instance()
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

    dryad_error generate_instances_until(dryad_voice* voice, dryad_time position)
    {
        if (!voice)
            return dryad_error_invalid_voice;

        dryad_motif_instance* last_instance = get_last_instance();

        dryad_time last_instance_end = 0;

        if (dryad_motif_instance* last_instance = get_last_instance())
            last_instance_end = last_instance->position + duration;

        dryad_score* score = static_cast<dryad_score*>(graph);
        if (!score)
            return dryad_error_node_not_in_graph;

        while (last_instance_end < position)
        {
            dryad_error error = voice->append_motif_instance(motif, last_instance);
            if (error)
                return error;

            last_instance_end = last_instance->position + duration;
        }

        return dryad_error_not_implemented;
    }

};

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

class dryad_score : public dryad_graph
{
public:
    DRYAD_CLASS_ID(dryad_score);

    dryad_score()
        : current_progression(nullptr)
        , current_scale(nullptr)
    {
    }

    dryad_voice* add_voice(int id, dryad_string name)
    {
        dryad_voice* voice = create<dryad_voice>(id, name);

        if (voice)
            voices.insert(voice);

        return voice;
    }

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
    dryad_error commit(dryad_time duration_to_commit)
    {
        dryad_score_frame* frame = find_last_committed_frame();

        if (!frame)
            frame = get_or_create_frame(0);

        dryad_time total_committed_time = frame->position + duration_to_commit;

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

    dryad_error dump(dryad_serialized_score& serialized_score)
    {
        return dryad_error_not_implemented;
    }

    dryad_score_frame* get_or_create_frame(dryad_time position)
    {
        dryad_score_frame* frame = find_frame_at_position(position);

        if (frame)
            return frame;

        frame = create<dryad_score_frame>(position);

        frames.insert(frame);

        return frame;
    }

    dryad_score_frame* find_frame_at_position(dryad_time position)
    {
        // Creating a dummy frame with the searched position to leverage std::find
        dryad_score_frame dummy_frame(position);

        auto it = frames.find(&dummy_frame);

        if (it == frames.end())
            return nullptr;

        return *it;
    }

    dryad_score_frame* find_last_committed_frame()
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

    dryad_progression* current_progression;
    dryad_scale* current_scale;
    dryad_set<dryad_voice*, dryad_voice::compare_by_id> voices;
    dryad_set<dryad_score_frame*, dryad_score_frame::compare_by_position> frames;

};

class dryad_exporter
{
public:
    virtual ~dryad_exporter()
    {
    }

    virtual dryad_error export_score(const dryad_serialized_score& score, dryad_string export_path) = 0;
};
