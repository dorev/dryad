#pragma once

#include "dryadcommon.h"
#include "phrase.h"

namespace dryad
{

class mode_graph;

class composer
{
public:

    composer();

    composer& set_mode(mode_graph* mode);
    composer& add_phrase(int id, phrase phrase);
    composer& set_phrase_sequence(std::initializer_list<int> phrase_list);

    void execute();

private:

    mode_graph*              _mode;
    std::map<int, phrase>    _phrases;
    std::vector<int>         _phrase_sequence;
    std::vector<phrase>      _song;

};

}