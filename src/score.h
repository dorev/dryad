#pragma once

#include "definitions.h"
#include "pitch.h"
#include "scorePosition.h"
#include <fstream>

struct Score
{
  std::map<int, ScorePosition> _score;
  int _divisionsValue;
  
  // Constructor
  Score(xml_document& xmlScore)
   : _score({})
   , _divisionsValue(0)
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
    for(auto scorePos : _score)
    {
      writer.StartObject();
        writer.Key("position");
        writer.Int(scorePos.first);
        writer.Key("notes");
        writer.StartArray();
        for(auto note : scorePos.second._notes)
        {
          writer.StartObject();
          writer.Key("step");
          writer.String(note._step.c_str());
          writer.Key("alter");
          writer.Int(note._alter);
          writer.Key("octave");
          writer.Int(note._octave);
          writer.Key("duration");
          writer.Int(note._duration);
          writer.Key("num");
          writer.Int(note._num);
          writer.EndObject();        
        }
        writer.EndArray();
      writer.EndObject();
    }
    writer.EndArray();
    writer.EndObject();

    return stringBuffer.GetString();
  }

  ScorePositionPtr operator[](int index)
  {
    return (_score.find(index) == _score.end())
      ? nullptr
      : std::make_shared<ScorePosition>(_score[index]);
  }

  bool writeToScore(Pitch& pitch, int pos)
  {
    ofstream log("scoreLog.txt", std::ios::out | std::ios::app);
    if(!pitch.isValid())
    {
      log << "!!! INVALID pitch " << pitch._num << "(" << pitch._step << "a" << pitch._alter << ") at measure " << pitch._measure << " pos " << pos << "\n";
      return false; 
    }

    log << "INSERTING pitch " << pitch._num << "(" << pitch._step << "a" << pitch._alter << ") at measure " << pitch._measure << " pos " << pos << "\n";
      
    return pitch.isValid() && _score[pos].insert(pitch);
  }

  void fillScore(xml_document& xmlScore)
  {
    for (auto& part : xmlScore.select_nodes("//part"))
    {
      int pos = 0;
      int prevPos = 0;

      for (auto& measure : part.node().children())
      {
        int shift = 0;

        for(auto& node : measure.children())
          parseMeasureNode(node, pos, prevPos, shift); 
      } 
    } 

    // Link position pointers
    ScorePositionPtr prev = nullptr;
    for(auto& position : _score)
    {
      ScorePosition& currentPos = position.second;
      ScorePositionPtr currentPtr = makeScorePosPtr(position.second);
      
      if(prev)
        prev->_next = currentPtr;
      
      currentPos._prev = prev;

      // Prepare for next iteration
      prev = currentPtr;
    }
  }

  void parseMeasureNode(xml_node& node, int& pos, int& prevPos, int& shift)
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

    // Write rest
    if(node.child("rest"))
    {
      //Pitch pitch;
      //pitch._duration = duration;
      //pitch._nodePtr = makeNodePtr(node);
      //writeToScore(pitch, pos);
      pos += duration;
      return;
    }

    // Create new note
    Pitch pitch; 
    if(!pitch.fromNode(node))
      throw "unable to parse Pitch from xml_node";

    // Write note in chord
    if(node.child("chord"))
    {
      // Write to previous position and don't increment pos
      writeToScore(pitch, prevPos);
      return;
    }

    // Write normal note
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
    _divisionsValue = maxDivision;

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
    for(auto scoreKV : _score)
    {
      int pos = scoreKV.first;
      auto scorePos = scoreKV.second;

      for(auto note : scorePos._notes)
        for(auto resonatingPos : findPosInRange(pos+1, pos + note._duration))
          resonatingPos->addResonating(makePitchPtr(note));
    }
  }

  std::set<ScorePositionPtr> findPosInRange(int rangeBegin, int rangeEnd)
  {
    std::set<ScorePositionPtr> result;

    for(int i = rangeBegin; i <= rangeEnd; ++i)
      if(_score.find(i) != _score.end())
        result.insert(makeScorePosPtr(_score[i]));

    return result;
  }

};
