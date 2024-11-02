#pragma once

namespace Dryad
{
    template<class T>
    constexpr bool IsPointer = std::is_pointer<T>::value;

    template<class T>
    constexpr bool IsFloatingPoint = std::is_floating_point<T>::value;
    
    template<class T>
    constexpr bool IsIntegral = std::is_integral<U>::value>;

    template<bool B, class T = void>
    using EnableIf = typename std::enable_if<B, T>::type;
}