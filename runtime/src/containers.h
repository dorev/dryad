#pragma once


#include <vector>

    template <class T>
    using Vector = std::vector<T>;

namespace Dryad
{
    template <class T>
    class Vector
    {
    private:
        std::vector<T> vector;

    public:
        void PushBack(const T& item)
        {
            vector.push_back(item);
        }

        void Clear(bool deallocate = false)
        {
            vector.clear();
            if(deallocate)
            {
                vector.shrink_to_fit();
            }
        }

        void Clean()
        {
            Clear(true);
        }

        bool Erase(const T& item)
        {
            for(auto itr = vector.begin(); itr != vector.end(); itr++)
            {
                if(item == *itr)
                {
                    vector.erase(itr);
                    return true;
                }
            }
            return false;
        }

        using iterator = typename std::vector<T>::iterator;

        auto begin()
        {
            return vector.begin();
        }

        auto end()
        {
            return vector.end();
        }
    };
}