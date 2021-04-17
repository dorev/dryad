#pragma once

namespace Dryad::Model
{

template <class T>
class Listed
{

public:

    inline T* Next() { return _next; }
    inline T* Prev() { return _prev; }

protected:

    T* _next;
    T* _prev;
};

} // namespace Dryad::Model