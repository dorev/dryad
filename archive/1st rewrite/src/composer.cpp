#include "composer.h"
#include "dryadutils.h"
#include "modegraph.h"
#include "musicxmlwriter.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

composer::composer()
    : _mode(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

composer& composer::set_mode(mode_graph* mode)
{
    _mode = mode;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

composer& composer::add_phrase(int id, phrase_t phrase)
{
    if (_phrases.find(id) != _phrases.end())
    {
        CRASH("Attempting to overwrite existing phrase");
    }

    phrase.set_parent(&_song);
    _phrases[id] = phrase;

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

composer& composer::set_phrase_sequence(std::initializer_list<int> phrase_list)
{
    for (int id : phrase_list)
    {
        if (_phrases.find(id) == _phrases.end())
        {
            CRASH("Adding unidentified phrase to sequence");
        }
    }

    _phrase_sequence = phrase_list;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

composer& composer::add_melody(int id, std::initializer_list<int> phrase_list)
{
    static std::vector<int> melody_durations = { HALF, HALF_DOTTED, WHOLE, 2 * WHOLE};

    _melodies[id] = melody_t(random::in(melody_durations), random::range(2, 6));

    for (int phrase : phrase_list)
    {
        _melodies_of_phrases[phrase].push_back(id);
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string composer::execute()
{
    if (_mode == nullptr ||
        _phrases.empty() ||
        _phrase_sequence.empty())
    {
        CRASH("Missing elements to execute composer");
    }

    // this could happen only once when composer is initialized
    _mode->generate_permutations();

    for (auto& [id, phrase] : _phrases)
    {
        phrase.set_progression(_mode->random_prog());
        phrase.fit_progression();
        
        for (int melody_id : _melodies_of_phrases[id])
        {
            phrase.add_melody(_melodies[melody_id]);
        }

        phrase.fit_melodies();
    }

    for (int phrase_id : _phrase_sequence)
    {
        _song.push_back(_phrases[phrase_id]);
    }

    // apply notes on progressions and offsets of song
    for (phrase_t& phrase : _song)
    {
        phrase.apply_scale(_mode->get_scale(), C4);
    }

    // render musicxml
    return musicxml_writer::render(&_song);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}