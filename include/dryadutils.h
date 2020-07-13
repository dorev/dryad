#pragma once

#include "dryadcommon.h"
#include <random>

#define LOG(x) std::cout << x << "\n"
#define CRASHLOG(x) { std::cout << "\n\n" << x << "\n --> " << __FILE__ << " l." << __LINE__ << "\n\n"; throw; }

namespace dryad
{

void uppercase(std::string& s);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class timer
{
public:

    timer();
    void reset();
    std::string stop();

private:

    std::chrono::time_point<std::chrono::steady_clock> _start;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class random
{
public:
    static int range(int low, int high);
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