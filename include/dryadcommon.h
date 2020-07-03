#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <functional>
#include <vector>
#include <deque>
#include <set>
#include <chrono>
using namespace std::chrono_literals;

namespace dryad
{

constexpr size_t MAX_PROG_LENGTH    = 128;
constexpr size_t MAX_BAR_DIVISION   = 4;

enum class accidental
{
    none,
    sharp,
    flat,
};

enum class triad
{
    none,
    major,
    minor,
    dim,
    aug,
};

enum class structural_pattern
{
    none,

    // 2 elements
    aaab,
    aabb,
    abab,

    // 3 elements
    aabc,
    abcc,
    abac,

    // 4 elements
    abcd,
};

}