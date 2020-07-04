#pragma once

#include "dryadcommon.h"
#include "phraselayout.h"

namespace dryad
{

class mode_graph;

class composer
{
public:

    composer();

    composer& set_mode(mode_graph* mode);
    composer& add_phrase(int id, phrase_layout phrase);
    composer& set_phrase_sequence(std::initializer_list<int> phrase_list);

    void execute();

private:

    mode_graph*                     _mode_graph;
    std::map<int, phrase_layout>    _phrases;
    std::vector<int>                _phrase_sequence;
    std::vector<phrase_layout>      _song;

};

}