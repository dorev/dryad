#pragma once

#include "types.h"

namespace Dryad
{

    struct SerializedNote
    {
        Time relativePosition;
        Time duration;
        NoteValue value;
    };

    struct SerializedVoice
    {
        String name;
        int id;
        Vector<SerializedVoice> notes;
    };

    struct SerializedScore
    {
        Vector<SerializedVoice> cachedVoices;
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
