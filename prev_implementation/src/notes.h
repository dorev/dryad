#pragma once

#include "definitions.h"

const int __notesIndex[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

int noteNum(std::string noteName)
{
  if (noteName.size() < 1 || noteName.size() > 2)
    return -1;
  else if (noteName == "C")
    return 0;
  else if (noteName == "C#" || noteName == "Db")
    return 1;
  else if (noteName == "D")
    return 2;
  else if (noteName == "D#" || noteName == "Eb")
    return 3;
  else if (noteName == "E")
    return 4;
  else if (noteName == "F")
    return 5;
  else if (noteName == "F#" || noteName == "Gb")
    return 6;
  else if (noteName == "G")
    return 7;
  else if (noteName == "G#" || noteName == "Ab")
    return 8;
  else if (noteName == "A")
    return 9;
  else if (noteName == "A#" || noteName == "Bb")
    return 10;
  else if (noteName == "B")
    return 11;
  else
    return -1;
}
std::string noteName(int noteNum, bool flat = false)
{
  switch (noteNum % 12)
  {
  case 0:
    return "C";
  case 1:
    return flat ? "Db" : "C#";
  case 2:
    return "D";
  case 3:
    return flat ? "Eb" : "D#";
  case 4:
    return "E";
  case 5:
    return "F";
  case 6:
    return flat ? "Gb" : "F#";
  case 7:
    return "G";
  case 8:
    return flat ? "Ab" : "G#";
  case 9:
    return "A";
  case 10:
    return flat ? "Bb" : "A#";
  case 11:
    return "B";
  default:
    return "";
  }
}
