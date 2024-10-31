#pragma once

#include <vector>
#include <string>

#include "dryad/definitions.h"

namespace Dryad
{

template <class T> using Vector = std::vector<T>;
using String = std::string;
using Cstr = const char*;
using Byte = uint8_t;
using Int = int;
using Uint = size_t;
using Id = Uint;
using MidiValue = Uint;

}