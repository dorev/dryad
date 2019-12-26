#pragma once

#include "definitions.h"
#include "score.h"


struct RuleCheckResult 
{
  bool rulePassed;
  std::set<NodePtr> badNotes;
  std::set<NodePtr> explaningNotes;
  std::string message;
};

using Rule = std::function<RuleCheckResult(ScorePosition&)>;
#define PASS { true, {}, {}, "" }

namespace Rules
{
  const Rule parallelFifths = [](ScorePosition& pos) -> RuleCheckResult
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
      { fifths.begin()->first->_nodePtr, fifths.begin()->second->_nodePtr },
      { prevFifths.begin()->first->_nodePtr, prevFifths.begin()->second->_nodePtr },
      "Consecutive fifths"
    };
  };

  const Rule parallelOctaves = [](ScorePosition& pos) -> RuleCheckResult
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
      { octaves.begin()->first->_nodePtr, octaves.begin()->second->_nodePtr },
      { prevOctaves.begin()->first->_nodePtr, prevOctaves.begin()->second->_nodePtr },
      "Consecutive octaves"
    };
  };



}
