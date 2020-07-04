#include "composer.h"
#include "dryadutils.h"
#include "modegraph.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

composer::composer()
    : _mode_graph(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

composer& composer::set_mode(mode_graph* mode)
{
    _mode_graph = mode;

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

composer& composer::add_phrase(int id, phrase_layout phrase)
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
    if (_mode_graph == nullptr ||
        _phrases.empty() ||
        _phrase_sequence.empty())
    {
        CRASHLOG("Missing elements to execute composer");
    }

    _mode_graph->generate_permutations();

    for (auto& [id, phrase] : _phrases)
    {
        //const progression& prog = _mode_graph->random_prog();

    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}