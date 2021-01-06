#pragma once

#include "includes.h"
#include "definitions.h"

namespace dryad
{

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

void get_equivalent_duration_pairs(int duration, std::vector<std::pair<int, int>>& solutions);

} // namespace dryad
