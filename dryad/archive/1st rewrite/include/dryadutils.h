#pragma once

#include "includes.h"
#include "dryaddefinitions.h"
#include "dryadmacros.h"
#include "dryadaliases.h"
#include "dryadrandom.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void uppercase(std::string& s);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int step_down_duration(int duration, const std::vector<int>& duration_vector);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int step_up_duration(int duration, const std::vector<int>& duration_vector);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void integer_to_step_and_alteration(int integer, const char*& output_step, int& output_alteration, accidental accidental_type = accidental::sharp);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* duration_to_note_type(int duration);

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

template<typename INTEGER, std::enable_if_t<std::is_integral_v<INTEGER>, int> = 0>
bool is_power_of_2(INTEGER n)
{
    u64 set_bits = 0;
    for (u64 b = 0; b < sizeof(INTEGER); ++b)
    {
        if (static_cast<u64>(n) & (1ULL << b) && set_bits++)
        {
            return false;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



}