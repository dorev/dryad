#pragma once



#include <variant>
#include <deque>
#include <string>

#include "containers.h"

namespace Dryad
{
    //
    // Aliases for standard types
    //

    using Byte = char;
    using Int8 = char;
    using UInt8 = unsigned char;
    using UInt32 = unsigned int;
    using Int32 = int;
    using String = std::string;

    //
    // Dryad basic types
    //

    using IdType = UInt32;
    using NoteValue = UInt8;
    using NoteVelocity = UInt8;
    using Time = float;
    using Tempo = float;
    using ScoreTime = UInt32;
}