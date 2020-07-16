#pragma once

#include "dryadcommon.h"

namespace dryad
{

class mode_graph;

class note
{
public:

private:

    int _duration; // based on degrees? think about how to transpose a melody to another mode
    int _pitch;
};


class melody
{
public:

    melody(mode_graph* mode, size_t duration);

    int get_duration(); // a good time to use reduce or accumulate!


    // generateVariation? (generate, stores and return a ref to a variations of the melody)

private:

    std::vector<note> _notes;

};

}