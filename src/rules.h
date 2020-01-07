#pragma once

#include "definitions.h"
#include "score.h"
#include "analysis.h"


struct RuleCheckResult 
{
  bool rulePassed;
  int measure;
  std::set<NodePtr> badNotes;
  std::set<NodePtr> explaningNotes;
  std::string message;
};

using RuleFunc = std::function<RuleCheckResult(const ScorePosition&)>;

struct Rule
{
  std::string name;
  RuleFunc func;

  bool operator<(Rule& other)
  {
    return name < other.name;
  }
};

// Constructing an empty RuleCheckResult with rulePassed = true;
#define PASS { true, -1, {}, {}, "" }

namespace Rules
{
  // --------------------------------------------------------------------------

  const Rule parallelFifths = 
  { 
    "parallel fifths", 
    [](const ScorePosition& pos) -> RuleCheckResult
    {
      if(pos._prev == nullptr)
        return PASS;

      auto fifths = findIntervalsInPos(7, pos);

      if(fifths.size() == 0)
        return PASS;
      
      auto prevFifths = findIntervalsInPos(7, *pos._prev);

      if(prevFifths.size() == 0)
        return PASS;

      return 
      {
        false,
        pos.measure(),
        { fifths.begin()->first->_nodePtr, fifths.begin()->second->_nodePtr },
        { prevFifths.begin()->first->_nodePtr, prevFifths.begin()->second->_nodePtr },
        "Consecutive fifths"
      };
    }
  };

  // --------------------------------------------------------------------------

  const Rule parallelOctaves = 
  {
    "parallel octaves",
    [](const ScorePosition& pos) -> RuleCheckResult
    {
      if(pos._prev == nullptr)
        return PASS;

      auto octaves = findIntervalsInPos(12, pos);

      if(octaves.size() == 0)
        return PASS;

      if(pos._prev->_scoreIndex == 636)
        std::cout << "!\n";
      
      auto prevOctaves = findIntervalsInPos(12, *pos._prev);

      if(prevOctaves.size() == 0)
        return PASS;

      return 
      {
        false,
        pos.measure(),
        { octaves.begin()->first->_nodePtr, octaves.begin()->second->_nodePtr },
        { prevOctaves.begin()->first->_nodePtr, prevOctaves.begin()->second->_nodePtr },
        "Consecutive octaves"
      };
    }
  };

  // --------------------------------------------------------------------------

  const Rule dasdasd = 
  {
    "template",
    [](const ScorePosition& pos) -> RuleCheckResult
    {
      return PASS;
    }
  };

  // --------------------------------------------------------------------------

  const Rule templateRule = 
  {
    "template",
    [](const ScorePosition& pos) -> RuleCheckResult
    {
      return PASS;
    }
  };



}
