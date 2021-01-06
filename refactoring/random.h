#pragma once

#include "includes.h"

// Helper functions to identify if a container has random-access capability
template<typename T>
using iterator_category_of = typename std::iterator_traits<typename T::iterator>::iterator_category;

template<typename T>
constexpr bool has_random_access = std::is_base_of_v<std::random_access_iterator_tag, iterator_category_of<T>>;

struct random
{
    template <class T>
    static T range(T min, T max)
    {
        if (min == max)
        {
            return min;
        }

        static thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<decltype(min + max)> distribution(min, max);
        return distribution(generator);
    }
};
