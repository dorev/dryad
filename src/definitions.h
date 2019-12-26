#pragma once

// std
#include <functional>
#include <iostream>
#include <set>
#include <string>
#include <cstring>
#include <map>
#include <memory>
#include <algorithm>

// extern
#include "pugixml/pugixml.hpp"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace std;

// cheating
using cstr = const char *;
using namespace pugi;
using NodePtr = std::shared_ptr<xml_node>;
#define makeNodePtr(xmlNode) std::make_shared<xml_node>(xmlNode)