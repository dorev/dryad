#pragma once

#include "containers.h"

namespace Dryad
{
    template <class T>
    void SafeDelete(T*& pointer)
    {
        if (pointer != nullptr)
        {
            delete pointer;
            pointer = nullptr;
        }
    }

    template <class T>
    void SafeDeleteArray(T*& pointer)
    {
        if (pointer != nullptr)
        {
            delete[] pointer;
            pointer = nullptr;
        }
    }

    template <class T>
    void CleanPointerVector(Vector<T*>& vector)
    {
        for(T* item : vector)
        {
            SafeDelete(item);
        }
        vector.Clean();
    }
}
