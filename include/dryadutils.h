#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <functional>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <cmath>
#include <chrono>
#include <random>
#include <utility>
#include <type_traits>
#include <memory>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Debugging macros

#define LOG(x) std::cout << x << "\n"
#define CRASHLOG(x) { std::cout << "\n\n" << x << "\n --> " << __FILE__ << " l." << __LINE__ << "\n\n"; *((int*)0xBAADD00D) = 0; throw; }

//#define PROFILING_ENABLED

#if defined(PROFILING_ENABLED)
    #define PROFILE(x) x;
#else
    #define PROFILE(x)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Utility macro

#define for_range(index_variable, limit) for(size_t index_variable = 0; index_variable < static_cast<size_t>(limit); ++index_variable)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace dryad
{

void uppercase(std::string& s);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int step_down_duration(int duration, const std::vector<int>& duration_vector);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int step_up_duration(int duration, const std::vector<int>& duration_vector);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<template<typename...> typename CONTAINER,
    typename T,
    typename HAS_BEGIN = decltype(std::declval<CONTAINER<T>>().begin()),
    typename HAS_END = decltype(std::declval<CONTAINER<T>>().end())>
    T& min_of(CONTAINER<T, std::allocator<T>> container)
{
    return *std::min_element(container.begin(), container.end());
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<template<typename...> typename CONTAINER,
    typename T,
    typename HAS_BEGIN = decltype(std::declval<CONTAINER<T>>().begin()),
    typename HAS_END = decltype(std::declval<CONTAINER<T>>().end())>
    T& max_of(CONTAINER<T, std::allocator<T>> container)
{
    return *std::max_element(container.begin(), container.end());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class dryad_timer
{
public:

    dryad_timer();
    void reset();
    std::string stop();
    double stop_ms();

private:

    std::chrono::time_point<std::chrono::steady_clock> _start;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename CONTAINER>
using iterator_category_of = typename std::iterator_traits<typename CONTAINER::iterator>::iterator_category;

template<typename CONTAINER>
constexpr bool has_random_access = std::is_base_of_v<std::random_access_iterator_tag, iterator_category_of<CONTAINER>>;

class random
{
public:
    static int range(int low, int high);
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
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename N, std::enable_if_t<std::is_integral_v<N>, int> = 0>
bool is_power_of_2(N n)
{
    size_t set_bits = 0;
    for (size_t b = 0; b < sizeof(N); ++b)
    {
        if (n & (1 << b) && set_bits++)
        {
            return false;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}