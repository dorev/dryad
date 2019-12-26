#pragma once

#include "definitions.h"
#include "score.h"


struct RuleCheckResult 
{
  bool rulePassed;
  int measure;
  std::set<NodePtr> badNotes;
  std::set<NodePtr> explaningNotes;
  std::string message;
};

using RuleFunc = std::function<RuleCheckResult(ScorePosition&)>;

struct Rule
{
  std::string name;
  RuleFunc func;
};


#define PASS { true, -1, {}, {}, "" }

namespace Rules
{
  const Rule parallelFifths = 
  { 
    "parallel fifths", 
    [](ScorePosition& pos) -> RuleCheckResult
    {
      if(pos._prev == nullptr)
        return PASS;

      auto fifths = pos.findInterval(7);

      if(fifths.size() == 0)
        return PASS;
      
      auto prevFifths = pos._prev->findInterval(7);

      if(prevFifths.size() == 0)
        return PASS;

      return 
      {
        false,
        pos._measure,
        { fifths.begin()->first->_nodePtr, fifths.begin()->second->_nodePtr },
        { prevFifths.begin()->first->_nodePtr, prevFifths.begin()->second->_nodePtr },
        "Consecutive fifths"
      };
    }
  };

  const Rule parallelOctaves = 
  {
    "parallel octaves",
    [](ScorePosition& pos) -> RuleCheckResult
    {
      if(pos._prev == nullptr)
        return PASS;

      auto octaves = pos.findInterval(12);

      if(octaves.size() == 0)
        return PASS;
      
      auto prevOctaves = pos._prev->findInterval(12);

      if(prevOctaves.size() == 0)
        return PASS;

      return 
      {
        false,
        pos._measure,
        { octaves.begin()->first->_nodePtr, octaves.begin()->second->_nodePtr },
        { prevOctaves.begin()->first->_nodePtr, prevOctaves.begin()->second->_nodePtr },
        "Consecutive octaves"
      };
    }
  };



}
