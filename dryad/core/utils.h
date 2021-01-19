#pragma once

#include "definitions.h"

namespace dryad
{

// CRTP class helper
template <typename sub_class, template<typename> typename crtp_class>
class crtp_helper
{
    // Convenience function to retrieve a reference to the inheriting class
    sub_class& get_child() { return static_cast<sub_class&>(*this); }

    // Private constructor resolves the ambiguity if more than once class implement the same crtp_class
    // Solves the diamond problem if a class inherits from multiple crtp_class
    friend crtp_class<sub_class>;
    crtp_helper() {}
};

struct random
{
    template <class T>
    static T range(T min, T max)
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
    static const auto& in(const T& container)
    {
        static thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<size_t> distribution(0, container.size() - 1);
        return container[distribution(generator)];
    }

    static bool coin_flip();
};

template <class T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
is_power_of_2(T integer)
{
    uint64_t set_bits = 0;

    if (integer < 0)
    {
        integer = std::abs(integer);
    }

    for (uint64_t bit = 0; bit < (sizeof(T) * 8); ++bit)
    {
        if ((int)integer & (1ULL << bit) && set_bits++)
        {
            return false;
        }
    }

    return true;
}

template <class T>
bool contains(const T& value, const std::vector<T>& vector)
{
    for (size_t i = 0; i < vector.size(); ++i)
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
reduce_vector(const std::vector<T>& vector)
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
std::shared_ptr<T> last(const std::vector<std::shared_ptr<T>>& vector)
{
    if (vector.size() == 0)
    {
        return nullptr;
    }

    return vector.back();
}

template <class T>
std::shared_ptr<T> first(const std::vector<std::shared_ptr<T>>& vector)
{
    if (vector.size() == 0)
    {
        return nullptr;
    }

    return vector.front();
}

void get_equivalent_duration_pairs(int duration, std::vector<std::pair<int, int>>& solutions);

} // namespace dryad
