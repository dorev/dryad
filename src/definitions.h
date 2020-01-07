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

// cheating
using cstr = const char *;
using namespace pugi;
using namespace rapidjson;
#define ALL(x) x.begin(),x.end()