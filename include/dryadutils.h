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

template <typename sub_class, template<typename> typename crtp_class>
class crtp_helper
{
    // Convenience function to retrieve a reference to the inheriting class
    sub_class& get_child() { return static_cast<sub_class&>(*this); }

    // Private constructor resolves the ambiguity if more than once class implement the same crtp_class
    crtp_helper() {}

    // Solves the diamond problem if a class inherits from multiple crtp_class
    friend crtp_class<sub_class>;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class id_provider
{
public:

    static uint64_t new_id() { return static_cast<uint64_t>(_id++); }

private:

    inline static std::atomic_uint64_t _id = 0;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename dryad_object_type>
class dryad_info : crtp_helper<dryad_object_type, dryad_info>
{
public:

    inline const char* get_name() const     { return _name; }
    inline const char* get_desc() const     { return _desc; }
    inline uint64_t get_id() const          { return _id; }

    inline void set_name(const char* name)  { _name = name; }
    inline void set_desc(const char* desc)  { _desc = desc; }

protected:

    dryad_info(const char* name = "", const char* desc = "")
        : _name(name)
        , _desc(desc)
        , _id(id_provider::new_id())
    {
        _created++;
        _alive++;
    }

    dryad_info(const dryad_info& other)
        : _name(other._name)
        , _desc(other._desc)
        , _id(id_provider::new_id())
    {
        _created++;
        _alive++;
    }

    ~dryad_info()
    {
        _alive--;
    }

private:

    const char* _name;
    const char* _desc;
    uint64_t _id;

    static std::atomic_uint64_t _created;
    static std::atomic_uint64_t _alive;
};

template <typename dryad_object_type>
std::atomic_uint64_t dryad_info<dryad_object_type>::_created = 0;

template <typename dryad_object_type>
std::atomic_uint64_t dryad_info<dryad_object_type>::_alive = 0;

}