#pragma once

#include "dryad/types.h"

namespace Dryad
{

// CRTP class helper
template <typename EnhancedClass, template<typename> typename CrtpClass>
class CrtpHelper
{
    // Convenience function to retrieve a reference to the inheriting class
    EnhancedClass& GetChild() { return static_cast<EnhancedClass&>(*this); }

    // Private constructor resolves the ambiguity if more than once class implement the same crtp_class
    // Solves the diamond problem if a class inherits from multiple crtp_class
    friend CrtpClass<EnhancedClass>;
    CrtpHelper() {}
};

struct Random
{
    template <class T>
    static T Range(T min, T max)
    {
        if (min == max)
        {
            return min;
        }
        if (max < min)
        {
            std::swap(min, max);
        }

        static thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<T> distribution(min, max);
        return distribution(generator);
    }

    template <class T>
    static const auto& In(const T& container)
    {
        static thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<Uint> distribution(0, container.size() - 1);
        return container[distribution(generator)];
    }

    static bool CoinFlip();
};

template <class T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
IsPowerOfTwo(T integer)
{
    Uint setBits = 0;

    if (integer < 0)
    {
        integer = std::abs(integer);
    }

    for (Uint bit = 0; bit < (sizeof(T) * 8); ++bit)
    {
        if ((Uint)integer & (1ULL << bit) && setBits++)
        {
            return false;
        }
    }

    return true;
}

template <class T>
bool Contains(const T& value, const Vector<T>& vector)
{
    for (Uint i = 0; i < vector.size(); ++i)
    {
        if (value == vector[i])
        {
            return true;
        }
    }

    return false;
}

template <class T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
ReduceVector(const Vector<T>& vector)
{
    return std::reduce(
        vector.begin(), vector.end(), 0,
        [](T acc, const T& value)
        {
            return acc + value;
        });
}

template <class T>
typename std::enable_if<std::is_same<std::weak_ptr<T>, decltype(T::next)>::value, std::shared_ptr<T>>::type
next(std::shared_ptr<T> item)
{
    return item->next.lock();
}

template <class T>
typename std::enable_if<std::is_same<std::weak_ptr<T>, decltype(T::previous)>::value, std::shared_ptr<T>>::type
previous(std::shared_ptr<T> item)
{
    return item->previous.lock();
}

template <class T>
std::shared_ptr<T> clone(std::shared_ptr<T> item)
{
    return std::make_shared<T>(*item);
}

template <class T>
std::shared_ptr<T> last(const Vector<std::shared_ptr<T>>& vector)
{
    if (vector.size() == 0)
    {
        return nullptr;
    }

    return vector.back();
}

template <class T>
std::shared_ptr<T> first(const Vector<std::shared_ptr<T>>& vector)
{
    if (vector.size() == 0)
    {
        return nullptr;
    }

    return vector.front();
}

void get_equivalent_duration_pairs(int duration, Vector<std::pair<int, int>>& solutions);

} // namespace Dryad