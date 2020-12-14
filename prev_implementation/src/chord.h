#pragma once

#include "definitions.h"

enum class ChordTriad
{
  Invalid = 0,
  Major,
  Minor,
  Sus2,
  Sus4,
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

enum class ChordExtension
{
  None,
  HalfDim,
  Six,
  Seven,
  MajorSeven
};

struct ChordQualities
{
  ChordTriad triads;
  ChordExtension extension;
};

using ChordIntervals = std::vector<int>;
const static std::map<ChordIntervals, ChordQualities> __chordsTypes({{{3, 4},     {ChordTriad::Minor, ChordExtension::None}},
                                                                     {{3, 4, 2},  {ChordTriad::Minor, ChordExtension::Six}},
                                                                     {{3, 4, 3},  {ChordTriad::Minor, ChordExtension::Seven}},
                                                                     {{3, 4, 4},  {ChordTriad::Minor, ChordExtension::MajorSeven}},
                                                                     {{4, 3},     {ChordTriad::Major, ChordExtension::None}},
                                                                     {{4, 3, 2},  {ChordTriad::Major, ChordExtension::Six}},
                                                                     {{4, 3, 3},  {ChordTriad::Major, ChordExtension::Seven}},
                                                                     {{4, 3, 4},  {ChordTriad::Major, ChordExtension::MajorSeven}},
                                                                     {{3, 3},     {ChordTriad::Dim,   ChordExtension::None}},
                                                                     {{3, 3, 3},  {ChordTriad::Dim,   ChordExtension::Seven}},
                                                                     {{3, 3, 4},  {ChordTriad::Dim,   ChordExtension::HalfDim}},
                                                                     {{4, 4},     {ChordTriad::Aug,   ChordExtension::None}},
                                                                     {{2, 5},     {ChordTriad::Sus2,  ChordExtension::None}},
                                                                     {{2, 5, 3},  {ChordTriad::Sus2,  ChordExtension::Seven}},
                                                                     {{2, 5, 4},  {ChordTriad::Sus2,  ChordExtension::MajorSeven}},
                                                                     {{5, 2},     {ChordTriad::Sus4,  ChordExtension::None}},
                                                                     {{5, 2, 3},  {ChordTriad::Sus4,  ChordExtension::Seven}},
                                                                     {{5, 2, 4},  {ChordTriad::Sus4,  ChordExtension::MajorSeven}}});

struct Chord
{
  std::set<int> _notes;
  std::set<int> _absoluteNotes;
  std::string _name;
  int _root;
  int _bass;
  ChordTriad _triads;
  ChordInversion _inversion;
  ChordExtension _extension;

  Chord(std::set<int> notes)
      : _notes(notes)
      , _name("")
      , _bass(*_notes.begin())
      , _triads(ChordTriad::Invalid)
      , _inversion(ChordInversion::Invalid)
      , _extension(ChordExtension::None)
  {
    for (int note : _notes)
      _absoluteNotes.insert(note % 12);
  }

  bool operator<(const Chord &other) const
  {
    return _name < other._name;

    return _bass < other._bass && _notes.size() < other._notes.size() && _inversion > other._inversion;
  }
};

std::string toString(const ChordTriad &input)
{
  switch (input)
  {
  case ChordTriad::Major:
    return "";
  case ChordTriad::Minor:
    return "m";
  case ChordTriad::Aug:
    return "+";
  case ChordTriad::Dim:
    return "-";
  case ChordTriad::Sus2:
    return "sus2";
  case ChordTriad::Sus4:
    return "sus4";
  default:
    return "";
  }
}

std::string toString(const ChordExtension &input)
{
  switch (input)
  {
  case ChordExtension::HalfDim:
    return "half-dim";
  case ChordExtension::Six:
    return "6";
  case ChordExtension::Seven:
    return "7";
  case ChordExtension::MajorSeven:
    return "M7";
  default:
    return "";
  }
}
