#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include <algorithm>
#include <functional>
#include <numeric>

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
#include <atomic>
#include <cctype>
#include <memory>

template <class sub_class, template<class> class crtp_class>
class crtp_helper
{
    // Convenience function to retrieve a reference to the inheriting class
    sub_class& get_sub_class() { return static_cast<sub_class&>(*this); }

    // Private constructor resolves the ambiguity if more than once class implement the same crtp_class
    // Solves the diamond problem if a class inherits from multiple crtp_class
    friend crtp_class<sub_class>;
    crtp_helper() {}
};

