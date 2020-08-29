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

        inline int get_offset() const { return _offset; }
        inline int get_duration() const { return _duration; }

    private:

        int _offset;
        int _duration;

    };

}