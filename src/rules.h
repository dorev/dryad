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
  std::string message;
};

using RuleFunc = std::function<RuleCheckResult(const ScorePosition&)>;


struct Rule
{
  Text title;
  Text description;
  RuleFunc func;

  bool operator<(const Rule& other) const
  {
    return title[Lang::fr] < other.title[Lang::fr];
  }
};

// Constructing an empty RuleCheckResult with rulePassed = true;
#define PASS { true, -1, {}, {}, "" }

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