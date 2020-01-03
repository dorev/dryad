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

using namespace rapidjson;
using namespace std;

// cheating
using cstr = const char *;
using namespace pugi;
using NodePtr = std::shared_ptr<xml_node>;
#define makeNodePtr(xmlNode) std::make_shared<xml_node>(xmlNode)
#define makePitchPtr(pitch) std::make_shared<Pitch>(pitch)
#define makeScorePosPtr(scorePos) std::make_shared<ScorePosition>(scorePos)


int noteNum(std::string noteName)
{
  if(noteName.size() < 1 || noteName.size() > 2)
    return -1;
  else if(noteName == "C")
    return 0;
  else if(noteName == "C#" || noteName == "Db")
    return 1;
  else if(noteName == "D")
    return 2;
  else if(noteName == "D#" || noteName == "Eb")
    return 3;
  else if(noteName == "E")
    return 4;
  else if(noteName == "F")
    return 5;
  else if(noteName == "F#" || noteName == "Gb")
    return 6;
  else if(noteName == "G")
    return 7;
  else if(noteName == "G#" || noteName == "Ab")
    return 8;
  else if(noteName == "A")
    return 9;
  else if(noteName == "A#" || noteName == "Bb")
    return 10;
  else if(noteName == "B")
    return 11;
  else
    return -1;  
}

std::map<std::string, int> noteNumber(
{
  {"", -1},
  {"C", 0},
  {"C#", 1},
  {"Db", 1},
  {"D", 2},
  {"D#", 3},
  {"Eb", 3},
  {"E", 4},
  {"F", 5},
  {"F#", 6},
  {"Gb", 6},
  {"G", 7},
  {"G#", 8},
  {"Ab", 8},
  {"A", 9},
  {"A#", 10},
  {"Bb", 10},
  {"B", 11}
});
