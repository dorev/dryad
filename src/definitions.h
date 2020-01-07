#pragma once

// std
#include <functional>
#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <cstring>
#include <cmath>
#include <map>
#include <memory>
#include <algorithm>

// extern
#include "pugixml/pugixml.hpp"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

using namespace std;

// cheating
using cstr = const char *;
using namespace pugi;
using namespace rapidjson;
#define ALL(x) x.begin(),x.end()

template<typename T1, typename T2>
bool contains(T1 iterable, T2 value)
{
  return std::find(ALL(iterable), value) != iterable.end();
}