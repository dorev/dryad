#include "dryadutils.h"

#include <algorithm>
#include <cctype>

namespace dryad
{

void uppercase(std::string& s)
{
    for (char& c : s)
        c = static_cast<char>(std::toupper(c));
}

}