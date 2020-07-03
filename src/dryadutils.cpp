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

}