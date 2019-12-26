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

using Rule = std::function<RuleCheckResult(ScorePositionPtr)>;

namespace Rules
{
  const Rule parallelFifths = [](ScorePositionPtr) -> RuleCheckResult
  {

  };



}
