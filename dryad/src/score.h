#pragma once

#include "types.h"
#include "graph.h"
#include "flag.h"

namespace dryad
{
    class dryad_score
    {

        // has frames
        // has progression

        dryad_graph graph;
    };

    class dryad_progression : public dryad_node
    {
        DRYAD_CLASS_ID(dryad_progression);



    };

    class dryad_scale : public dryad_node
    {
        DRYAD_CLASS_ID(dryad_scale);


    };

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
        DRYAD_CLASS_ID(dryad_chord);

        int root;
        dryad_degree degree;
        dryad_chord_quality qualities;
        dryad_accidental accidental;
    };

    class dryad_score_frame : public dryad_node
    {
        DRYAD_CLASS_ID(dryad_score_frame);

        int position;
        dryad_score* get_score();
    };

    class dryad_voice : public dryad_node
    {
        DRYAD_CLASS_ID(dryad_voice);
    };

    class dryad_motif : public dryad_node
    {
        DRYAD_CLASS_ID(dryad_motif);

    };

    class dryad_motif_note : public dryad_node
    {
        DRYAD_CLASS_ID(dryad_motif_note);

        dryad_motif* get_motif();
        int duration;
        int relative_value;
    };

    class dryad_motif_instance : public dryad_node
    {
        DRYAD_CLASS_ID(dryad_motif_instance);

        dryad_chord* get_chord();
        dryad_motif* get_motif();
        dryad_score_frame* get_score_frame();
        dryad_voice* get_voice();
    };

    class dryad_note : public dryad_node
    {
        DRYAD_CLASS_ID(dryad_note);

        dryad_motif_note* get_motif_note();
        dryad_motif_instance* get_motif_instance();
        dryad_motif* get_motif();
        dryad_voice* get_voice();
        dryad_score_frame* get_score_frame();
        int duration;
        int value;
    };

}
