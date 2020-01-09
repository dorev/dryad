#pragma once

#include "definitions.h"
#include "pitch.h"
#include "scorePosition.h"

struct Score
{
  std::map<int, ScorePosition> score;
  int divisionsValue;
  const xml_document* xml;
  
  Score(xml_document& xmlScore)
   : score()
   , xml(&xmlScore)
   , divisionsValue(0)
  {
    uniformizeDivisions(xmlScore);
    fillScore(xmlScore);
    updateResonatingNotes();
  }

  std::string serialize()
  {
    StringBuffer stringBuffer;
    PrettyWriter<StringBuffer> writer(stringBuffer);

    writer.StartObject();
      writer.Key("score");
      writer.StartArray();
        for(auto scorePos : score)
        {
          writer.StartObject();
            writer.Key("position"); writer.Int(scorePos.first);
            writer.Key("notes");
            writer.StartArray();
              for(auto note : scorePos.second.notes)
              {
                writer.StartObject();
                  writer.Key("step");     writer.String(note.step.c_str());
                  writer.Key("alter");    writer.Int(note.alter);
                  writer.Key("octave");   writer.Int(note.octave);
                  writer.Key("duration"); writer.Int(note.duration);
                  writer.Key("num");      writer.Int(note.num);
                writer.EndObject();        
              }
            writer.EndArray();
          writer.EndObject();
        }
      writer.EndArray();
    writer.EndObject();

    return stringBuffer.GetString();
  }

  const ScorePosition* operator[](int index) const
  {
    auto result = score.find(index);
    return (result == score.end())
      ? nullptr
      : &result->second;
  }

  bool writeToScore(Pitch& pitch, int pos)
  {
    if(!pitch.isValid())
      return false;

    return score[pos].insert(pitch);
  }

  void fillScore(xml_document& xmlScore)
  {
    for (auto& part : xmlScore.select_nodes("//part"))
    {
      // Longer living value to keep track of position
      // in part used by parseMeasureNode
      int pos = 0;
      int prevPos = 0;

      for (auto& measure : part.node().children())
      {
        // Longer living value to keep track of backup/forward 
        // nodes used by parseMeasureNode
        int shift = 0;

        for(auto& node : measure.children())
          parseMeasureNode(node, pos, prevPos, shift); 
      } 
    } 

    // Link prev/next position pointers
    ScorePosition* prev = nullptr;
    for(auto& position : score)
    {
      ScorePosition& currentPos = position.second;
      ScorePosition* currentPtr = &position.second;
      
      if(prev)
        prev->_next = currentPtr;
      
      currentPos._prev = prev;

      // Prepare for next iteration
      prev = currentPtr;
    }
  }

  void parseMeasureNode(const xml_node& node, int& pos, int& prevPos, int& shift)
  {
    // Check for position shift
    if(!strcmp(node.name(),"backup"))
    {
      shift -= node.child("duration").text().as_int();
      return;
    }
    if(!strcmp(node.name(),"forward"))
    {
      shift += node.child("duration").text().as_int();
      return;
    }
    
    if(shift)
    {
      pos += shift;
      shift = 0;
    }

    if(strcmp(node.name(),"note"))
      return;
    
    // Skip if not has no duration
    int duration = node.child("duration").text().as_int();          
    if(duration == 0)
      return;

    // Rest
    if(node.child("rest"))
    {
      pos += duration;
      return;
    }

    // Create new note
    Pitch pitch; 
    if(!pitch.fromNode(node))
      throw "unable to parse Pitch from xml_node";

    // Chord
    if(node.child("chord"))
    {
      // Write to previous position and don't increment pos
      writeToScore(pitch, prevPos);
      return;
    }

    // Write note
    writeToScore(pitch, pos);
    prevPos = pos;
    pos += duration;
  }

  void uniformizeDivisions(xml_document& xml)
  {
    std::map<size_t, int> divisions = {};
    std::map<size_t, int> divisionFactors = {};

    // Find all division nodes
    for(auto& division : xml.select_nodes("//divisions"))
      divisions[division.node().offset_debug()] = division.node().text().as_int();

    int maxDivision = std::max_element(divisions.begin(), divisions.end())->second;
    divisionsValue = maxDivision;

    // Set all factors to normalize quarter-note durations
    for(auto& division : divisions)
      divisionFactors[division.first] = maxDivision / division.second;

    // Set all division nodes to the same value
    for(auto& division : xml.select_nodes("//divisions"))
      division.node().text().set(std::to_string(maxDivision).c_str());

    // Adjust all durations of the document
    for(auto& duration : xml.select_nodes("//duration"))
    {
      size_t offset = duration.node().offset_debug();
      int value = duration.node().text().as_int();

      // Find modifier to apply
      auto revItr = std::find_if
      (
        divisionFactors.rbegin(), 
        divisionFactors.rend(), 
        [&](auto& kv) { return kv.first < offset; }
      );

      if(revItr == divisionFactors.rend())
        throw "Iterator should never reach .rend() because "
              "a 'duration' node should never be before "
              "the first 'divisions' node";

      // Update duration value
      duration.node().text().set(std::to_string(value * (*revItr).second).c_str());
    }
  }

  void updateResonatingNotes()
  {
    for(auto& scoreKV : score)
    {
      int pos = scoreKV.first;
      auto& scorePos = scoreKV.second;

      // For every note, add self as 'resonating' in
      // existing positions within the range of their duration
      for(auto& note : scorePos.notes)
        for(auto resonatingPos : findPosInRange(pos+1, pos + note.duration-1))
          score[resonatingPos].addResonating(&note);
    }
  }

  void updateLikelyScale()
  {
    // trouver les indications aux armatures
    // se considérer en do, qu'on soit en Do majeur, Do mineur melo asc, harmo, naturel, etc...
    // regarder vers le futur? comment on explique les modulations?!?!

    // how do we deal with likely scales

    for(auto& scoreKV : score)
    {
      int pos = scoreKV.first;
      auto& scorePos = scoreKV.second;
    }

  }

  std::set<int> findPosInRange(int rangeBegin, int rangeEnd) const
  {
    std::set<int> result;

    // Scan for existing key values among the requested range
    for(int i = rangeBegin; i < rangeEnd; ++i)
      if(score.find(i) != score.end())
        result.insert(i);

    return result;
  }

};
