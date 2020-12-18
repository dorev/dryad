#pragma once

#include "dryadutils.h"

namespace dryad
{
    class note_t
    {
    public:

        note_t(int offset = 0, int duration = QUARTER)
            : _offset(offset)
            , _duration(duration)
        {}

        inline int get_offset() const { return _offset; }
        inline int get_duration() const { return _duration; }

        void set_offset(int offset) { _offset = offset; }
        void set_duration(int duration) { _duration = duration; }
        void reduce_duration(int reduction) { _duration -= reduction; }
        void extend_duration(int extension) { _duration += extension; }
        inline void set_pitch(int pitch) { _pitch = pitch; }

    private:

        int _offset;
        int _duration;
        int _pitch;

    };

}