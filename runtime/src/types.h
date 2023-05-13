#pragma once

#include <variant>
#include <vector>
#include <map>
#include <deque>
#include <string>

namespace Dryad
{
    //
    // Aliases for standard types
    //

    using Byte = char;
    using Int8 = char;
    using UInt8 = unsigned char;
    using UInt32 = unsigned int;
    using String = std::string;
    template <class T>
    using Vector = std::vector<T>;
    template <class K, class V>
    using Map = std::map<K, V>;
    template <class T>
    using Deque = std::deque<T>;
    template <class... T>
    using Variant = std::variant<T...>;

    //
    // Dryad basic types
    //

    using IdType = UInt32;
    using NoteValue = UInt8;
    using ScaleOffsets = NoteValue[7];
}