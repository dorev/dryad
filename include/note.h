#pragma once

#include "dryadutils.h"

namespace dryad
{
    class note
    {
    public:

        note(int offset = 0, int duration = QUARTER)
            : _offset(offset)
            , _duration(duration)
        {}

    private:

        int _offset;
        int _duration;

    };

}