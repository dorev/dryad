#pragma once

#include "dryadutils.h"
#include "phrase.h"
#include "melody.h"

namespace dryad
{

class mode_graph;

class composer
{
public:

    composer();

    composer& set_mode(mode_graph* mode);
    composer& add_phrase(int id, phrase_t phrase = phrase_t());
    composer& add_melody(int id, std::initializer_list<int> phrase_list);
    composer& set_phrase_sequence(std::initializer_list<int> phrase_list);

    void execute();

private:

    mode_graph*                      _mode;
    std::map<int, phrase_t>            _phrases;
    std::vector<int>                 _phrase_sequence;
    std::map<int, melody_t>            _melodies;
    std::map<int, std::vector<int>>  _melodies_of_phrases;
    std::vector<phrase_t>              _song;

};

}