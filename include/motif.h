#pragma once

#include "dryadutils.h"
#include "dryadinfo.h"
#include "includes.h"

namespace dryad
{
namespace model
{

class note;
class position;
class progression;
class degree;
class degree_id{};
class voice;
class motif;
class scale;
class measure;
class phrase;
class score;
class harmony_graph;
class harmony_node;
class session;

class note
{
    // Members
    int degree_offset;
    const char* step;
    int alteration;
    int octave;
    int duration;

    // References
    std::shared_ptr<voice> associated_voice;
    std::shared_ptr<motif> associated_motif;
    std::shared_ptr<position> parent_position;

    // Go to next or previous note of same voice
    static std::shared_ptr<note> next(note*) {}
    static std::shared_ptr<note> prev(note*) {}

    // Define note concrete value
    static void make_concrete(note*, harmony_node*) {}
};

class position
{
    // Members
    std::vector<note> notes;

    // References
    std::shared_ptr<degree> associated_degree;
    std::shared_ptr<measure> parent_measure;

    // Go to next position of song
    static std::shared_ptr<position> next(position*) {}
    static std::shared_ptr<position> prev(position*) {}
};

class motif_variation
{
    // References
    std::shared_ptr<motif> parent_motif;
    std::vector<note> notes;
};

class motif
{
    std::vector<motif_variation> variations;
};

class voice
{
    // References
    std::shared_ptr<score> parent_score;
};

class measure
{
    // Members
    std::vector<position> positions;

    // References
    std::vector<std::shared_ptr<degree>> degrees;
    std::shared_ptr<phrase> parent_phrase;

    // Go to next measure of song
    static std::shared_ptr<measure> next(measure*) {}
    static std::shared_ptr<measure> prev(measure*) {}
};

class phrase
{
    // Members
    std::vector<measure> mesures;

    // References
    std::vector<std::shared_ptr<degree>> degrees;
    std::shared_ptr<score> parent_score;

    // Go to next phrase of song
    static std::shared_ptr<phrase> next(phrase*) {}
    static std::shared_ptr<phrase> prev(phrase*) {}
};

class score
{
    // Members
    std::vector<motif> motifs;
    std::vector<voice> voices;
    std::vector<phrase> phrases;

    // References
    std::shared_ptr<session> parent_session;

    static void apply_scale(score* score, scale* scale) {}
    static void render_musicxml(score* score);
};

class degree
{
    // References
    std::shared_ptr<scale> parent_scale;
};

class scale
{
    // Members
    std::vector<note> intervals;
    std::vector<degree> degrees;
};

class harmony_node
{
    // Members
    int id;
    int inversion;
    int alteration;
    int modulation;
    std::vector<int> edges;

    // References
    std::shared_ptr<harmony_graph> parent_harmony_graph;
    std::shared_ptr<degree> associated_degree;
};

class harmony_graph
{
    // Members
    std::vector<harmony_node> nodes;
};

class session
{
    // Members
    std::vector<score> scores;
    harmony_graph graph;
};

} // namespace model
} // namespace dryad