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
#include "pugixml.hpp"

// cheating
using cstr = const char *;
using namespace pugi;
using NodePtr = std::shared_ptr<xml_node>;
#define makeNodePtr(xmlNode) std::make_shared<xml_node>(xmlNode)