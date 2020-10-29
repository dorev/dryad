#include "dryadutils.h"

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

int step_down_duration(int duration, const std::vector<int>& duration_vector)
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
        CRASH("Illegal duration");
    }

    return duration_vector[i > 0 ? --i : 0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int step_up_duration(int duration, const std::vector<int>& duration_vector)
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
        CRASH("Illegal duration");
    }

    return duration_vector[i < duration_vector.size() - 1 ? ++i : duration_vector.size() - 1];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

dryad_timer::dryad_timer()
    : _start(std::chrono::steady_clock::now())
{}

void dryad_timer::reset()
{
    _start = std::chrono::steady_clock::now();
}

std::string dryad_timer::stop()
{
    std::chrono::duration<double> diff = std::chrono::steady_clock::now() - _start;

    std::stringstream ss;
    ss << std::fixed << std::setprecision(6) << diff.count() * 1000 << " ms";

    return ss.str();
}

double dryad_timer::stop_ms()
{
    std::chrono::duration<double> diff = std::chrono::steady_clock::now() - _start;

    return diff.count() * 1000;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}