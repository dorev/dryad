#pragma once

#include "definitions.h"

enum class ChordTriads
{
  Invalid = 0,
  Maj,
  Min,
  Dim,
  Aug
};

enum class ChordInversion
{
  Invalid = 0,
  Root,
  Six,
  FourSix,
  NotImplementedYet 
};

enum class ChordVariations
{
  Sus2,
  Sus4,
  Six,
  Seven,
  MajorSeven
};

struct Chord
{
  std::set<int> _notes;
  std::set<int> _pureNotes;
  std::string _name;
  int _root;
  int _bass;
  ChordTriads _triads;
  ChordInversion _inversion;
  std::set<ChordVariations> _variations;

  Chord(std::set<int> notes) 
    : _notes(notes)
    , _name("")
    , _bass(*_notes.begin())
    , _triads(ChordTriads::Invalid)
    , _inversion(ChordInversion::Invalid)
    , _variations({})
  {
    for(int note : _notes)
      _pureNotes.insert(note % 12);    
  }

  bool operator<(const Chord& other) const
  {
    return _bass < other._bass;
  }
};

std::string toString(const ChordTriads& input)
{
  switch(input)
  {
    case ChordTriads::Maj : return "";
    case ChordTriads::Min : return "m";
    case ChordTriads::Aug : return "+";
    case ChordTriads::Dim : return "-";
    default : return "";
  }
}

std::string toString(const ChordVariations& input)
{
  switch(input)
  {
    case ChordVariations::Sus2 :        return "sus2";
    case ChordVariations::Sus4 :        return "sus4";
    case ChordVariations::Six :         return "6";
    case ChordVariations::Seven :       return "7";
    case ChordVariations::MajorSeven :  return "M7";
    default : return "";
  }
}
