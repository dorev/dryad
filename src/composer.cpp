#include "composer.h"
#include "dryadutils.h"
#include "modegraph.h"

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

composer& composer::add_phrase(int id, phrase phrase)
{
    if (_phrases.find(id) != _phrases.end())
    {
        CRASHLOG("Attempting to overwrite existing phrase");
    }

    _phrases[id] = phrase;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

composer& composer::set_phrase_sequence(std::initializer_list<int> phrase_sequence)
{
    for (int id : phrase_sequence)
    {
        if (_phrases.find(id) == _phrases.end())
        {
            CRASHLOG("Adding unidentified phrase to sequence");
        }
    }

    _phrase_sequence = phrase_sequence;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void composer::execute()
{
    if (_mode == nullptr ||
        _phrases.empty() ||
        _phrase_sequence.empty())
    {
        CRASHLOG("Missing elements to execute composer");
    }

    _mode->generate_permutations();

    // generate melodies

    for (auto& [id, phrase] : _phrases)
    {
        phrase.apply_progression(_mode->random_prog());
    }

    for (int phrase_id : _phrase_sequence)
    {
        _song.push_back(_phrases[phrase_id]);
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}