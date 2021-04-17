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
using ShortInt = int8_t;
using Uint = size_t;
using MidiValue = Uint;
using Id = Uint;
using Int = int;

}