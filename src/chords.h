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

std::string toString(const ChordInversion& input)
{
  switch(input)
  {
    case ChordInversion::Invalid :            return "Invalid";
    case ChordInversion::Root :               return "Root";
    case ChordInversion::Six :                return "Six";
    case ChordInversion::FourSix :            return "FourSix";
    case ChordInversion::NotImplementedYet :  return "NotImplementedYet";
    default : return "";

  }
}
