#pragma once

#include "dryadutils.h"
#include "voice.h"

namespace dryad
{

class degree_node;

class bar : dryad_info<bar>
{
public:

    bar();

    inline void insert_degree(degree_node* degree) { _progression.push_back(degree); }
    inline size_t progression_size() { return _progression.size(); }

    void insert_note(int offset, int duration);

private:

    progression _progression;
    voice _voice;
};

}