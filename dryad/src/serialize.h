#pragma once

#include "types.h"

namespace Dryad
{

    struct SerializedNote
    {
        Time position;
        Time duration;
        NoteValue noteValue;
    };

    struct SerializedVoice
    {
        String name;
        int id;
        Vector<SerializedNote> notes;
    };

    struct SerializedScore
    {
        Vector<SerializedVoice> voices;
    };

    class IExporter
    {
    public:
        virtual ~IExporter()
        {
        }

        virtual Error exportScore(const SerializedScore& score, String export_path) = 0;
    };

} // namespace Dryad
