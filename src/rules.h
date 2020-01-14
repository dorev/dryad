#pragma once

#include "definitions.h"
#include "score.h"
#include "analysis.h"
#include "text.h"

struct RuleCheckResult 
{
  bool rulePassed;
  int measure;
  std::set<const xml_node*> badNotes;
  std::set<const xml_node*> explaningNotes;
  Text message;
};

using RuleFunc = std::function<RuleCheckResult(const ScorePosition&)>;

struct Rule
{
  Text _title;
  Text _description;
  RuleFunc _func;

  bool operator<(const Rule& other) const
  {
    return _title[Lang::fr] < other._title[Lang::fr];
  }
};

void rulesSplit(std::set<std::string>& out, std::string rulesString, std::string delimiter)
{
  int next = 0;
  int last = 0;

  while((next = rulesString.find(delimiter, last)) != std::string::npos)
  {
    out.insert(rulesString.substr(last, next - last));
    last = next + delimiter.size();
  }

  if(last < rulesString.size())
    out.insert(rulesString.substr(last));
}


// Constructing an empty RuleCheckResult with rulePassed = true;
#define PASS { true, -1, {}, {}, {{{}}} }

namespace
{
  // --------------------------------------------------------------------------

  const Rule parallelFifths = 
  { 
    {{
      {Lang::fr, "Quintes parallèles"},
      {Lang::en, "Parallel fifths"}
    }},
    {{
      {Lang::fr, "Tu devrais pas faire ça selon des vieux"},
      {Lang::en, "Old people say you shouldn't do that"}
    }}, 
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
        pos._measure,
        { fifths.begin()->first->_nodePtr, fifths.begin()->second->_nodePtr },
        { prevFifths.begin()->first->_nodePtr, prevFifths.begin()->second->_nodePtr },
        {{
          {Lang::fr, "Quintes parallèles consécutives trouvées"},
          {Lang::en, "Consecutive parallel fifths found"}
        }}
      };
    }
  };

  // --------------------------------------------------------------------------

  const Rule parallelOctaves = 
  {
    {{
      {Lang::fr, "Octaves parallèles"},
      {Lang::en, "Parallel octaves"}
    }},
    {{
      {Lang::fr, "Tu devrais pas faire ça selon des vieux"},
      {Lang::en, "Old people say you shouldn't do that"}
    }}, 
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
        pos._measure,
        { octaves.begin()->first->_nodePtr, octaves.begin()->second->_nodePtr },
        { prevOctaves.begin()->first->_nodePtr, prevOctaves.begin()->second->_nodePtr },
        {{
          {Lang::fr, "Octaves parallèles consécutives trouvées"},
          {Lang::en, "Consecutive parallel octaves found"}
        }}
      };
    }
  };

  // --------------------------------------------------------------------------

  const Rule dasdasd = 
  {
    {{
      {Lang::fr, ""},
      {Lang::en, ""}
    }},
    {{
      {Lang::fr, ""},
      {Lang::en, ""}
    }}, 
    [](const ScorePosition& pos) -> RuleCheckResult
    {
      return PASS;
    }
  };

  // --------------------------------------------------------------------------

  const Rule templateRule = 
  {
    {{
      {Lang::fr, ""},
      {Lang::en, ""}
    }},
    {{
      {Lang::fr, ""},
      {Lang::en, ""}
    }}, 
    [](const ScorePosition& pos) -> RuleCheckResult
    {
      return PASS;
    }
  };

}

std::map<std::string, const Rule> __rules =
{
  { "parallelFifths", ::parallelFifths },
  { "parallelOctaves", ::parallelOctaves }
};


std::set<Rule> buildRuleSet(std::set<std::string>& rulesList)
{
  std::set<Rule> ruleSet;

  for(auto& ruleName : rulesList)
    if(__rules.find(ruleName) != __rules.end())
      ruleSet.insert(__rules[ruleName]);

  return ruleSet;
}