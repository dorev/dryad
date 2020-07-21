#include "dryadutils.h"

#include <cctype>
#include <sstream>

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void uppercase(std::string& s)
{
    for (char& c : s)
    {
        c = static_cast<char>(std::toupper(c));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void step_down_duration(int& duration, const std::vector<int>& duration_vector)
{
    size_t i = 0;

    for (i; i < duration_vector.size(); ++i)
    {
        if (duration == duration_vector[i])
        {
            break;
        }
    }

    if (i >= duration_vector.size())
    {
        CRASHLOG("Illegal duration");
    }

    duration = duration_vector[i > 0 ? --i : 0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void step_up_duration(int& duration, const std::vector<int>& duration_vector)
{
    size_t i = 0;

    for (i; i < duration_vector.size(); ++i)
    {
        if (duration == duration_vector[i])
        {
            break;
        }
    }

    if (i >= duration_vector.size())
    {
        CRASHLOG("Illegal duration");
    }

    duration = duration_vector[i < duration_vector.size() - 1 ? ++i : duration_vector.size() - 1];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

timer::timer()
    : _start(std::chrono::steady_clock::now())
{}

void timer::reset()
{
    _start = std::chrono::steady_clock::now();
}

std::string timer::stop()
{
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = end - _start;

    std::stringstream ss;
    ss << std::fixed << std::setprecision(6) << diff.count() * 1000 << " ms";

    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int random::range(int min, int max)
{
    static thread_local std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int random::normal_int(float mean, float stddev)
{
    static thread_local std::mt19937 generator;
    std::normal_distribution<float> distribution(mean, stddev);
    return static_cast<int>(distribution(generator));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool random::coin_flip()
{
    static thread_local std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(0, 1);
    return distribution(generator) > 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}