#pragma once

namespace Dryad
{
    //
    // EnableIf
    //
    /* Usage:
    template <typename U = ValueType, typename = EnableIf<!std::is_pointer<U>::value>>
    bool Contains(const U& item) const
    {
        ...
    }
    */

    template<bool B, class T = void>
    using EnableIf = typename std::enable_if<B, T>::type;
}