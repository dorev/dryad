#pragma once

#include "utils.h"

namespace dryad
{

template <class type>
struct monitor_count : crtp_helper<type, monitor_count>
{
    monitor_count()
    {
        alive_count++;
    }

    ~monitor_count()
    {
        alive_count--;
    }

    static uint64_t get_count() { return alive_count; }

private:

    static uint64_t alive_count;
};

template <typename type>
uint64_t monitor_count<type>::alive_count = 0;

} // namespace dryad