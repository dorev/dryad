#pragma once

#include "dryadcommon.h"

#define LOG(x) std::cout << x << "\n"
#define CRASHLOG(x) { std::cout << "\n\n" << x << "\n --> " << __FILE__ << " l." << __LINE__ << "\n\n"; exit(1); }

namespace dryad
{

    void uppercase(std::string& s);

}