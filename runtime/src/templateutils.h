#pragma once

namespace Dryad
{
    template<class T>
    constexpr bool IsPointer = std::is_pointer<T>::value;

    template<bool B, class T = void>
    using EnableIf = typename std::enable_if<B, T>::type;
}