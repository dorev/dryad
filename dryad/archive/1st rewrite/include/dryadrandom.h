#pragma once

#include "includes.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Helper functions to identify if a container has random-access capability

template<typename CONTAINER>
using iterator_category_of = typename std::iterator_traits<typename CONTAINER::iterator>::iterator_category;

template<typename CONTAINER>
constexpr bool has_random_access = std::is_base_of_v<std::random_access_iterator_tag, iterator_category_of<CONTAINER>>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class random
{
public:
    static int normal_int(float mean, float stddev);
    static bool coin_flip();

    // returns a random element of a randomly-accessible container
    template<template<typename...> typename CONTAINER,
        typename T,
        std::enable_if_t<has_random_access<CONTAINER<T>>, int> = 0>
    static const T& in(const CONTAINER<T>& c)
    {
        static thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<size_t> distribution(0, c.size() - 1);
        return c[distribution(generator)];
    }

    template <typename T, typename U>
    static auto range(T min, U max) -> decltype(min + max)
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}