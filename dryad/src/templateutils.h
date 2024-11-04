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

    template <class T>
    using Decay = typename std::decay<T>::type;

    template <class Base, class Derived>
    constexpr bool IsBaseOf = std::is_base_of<Base, Derived>::value;

    template <class T, class U>
    constexpr bool IsSame = std::is_same<T, U>::value;
}