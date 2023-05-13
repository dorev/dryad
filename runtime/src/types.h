#pragma once

#include <variant>
#include <vector>
#include <map>
#include <deque>
#include <string>
#include <memory>

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
    template <class T>
    using SharedPtr = std::shared_ptr<T>;

    template <class T, class... Args>
    SharedPtr<T> MakeShared(Args... args)
    {
        return std::make_shared<T>(new T(std::forward<Args>(args)...));
    }

    //
    // Dryad basic types
    //

    using IdType = UInt32;
    using NoteValue = UInt8;
    using ScaleOffsets = NoteValue[7];
}