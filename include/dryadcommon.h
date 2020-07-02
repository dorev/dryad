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

    constexpr int SHARP = 1;
    constexpr int FLAT = -1;

    constexpr int MAJOR = 0;
    constexpr int MINOR = 1;
    constexpr int DIM = 2;
    constexpr int AUG = 3;

    constexpr size_t HARD_MAX_PROG_LENGTH = 128;

}