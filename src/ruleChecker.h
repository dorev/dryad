#pragma once

#include <vector>

#include "rules.h"
#include "score.h"

std::vector<RuleCheckResult> checkRulesOnScore(const std::vector<Rule>& ruleSet, const Score& score )
{
  std::cout << "Analyzing with ruleset : \n";
  for(auto& rule : ruleSet)
    std::cout << "  - " << rule.name << "\n";

  std::vector<RuleCheckResult> errors;

  for(auto& pos : score._score)
  {
    for(auto& rule : ruleSet)
    {
      auto result = rule.func(pos.second);
      if(!result.rulePassed)
        errors.push_back(result);
    }    
  }

  return errors;
}