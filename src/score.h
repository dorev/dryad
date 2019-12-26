#pragma once

#include "definitions.h"
#include "pitch.h"

struct ScorePosition 
{  
  std::set<Pitch> _notes;
  std::set<Pitch> _resonatingNotes;

  bool insert(Pitch& pitch)
  {    
    return _notes.emplace(pitch).second;
  }
};

using ScorePositionPtr = std::shared_ptr<ScorePosition>;

struct Score
{
  std::map<int, ScorePosition> _score;
  
  // Constructor
  Score(xml_document& xmlScore)
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
    return _score[pos].insert(pitch);
  }

  void updateResonatingNotes()
  {
  }

  void fillScore(xml_document& xmlScore)
  {
    for (auto& part : xmlScore.select_nodes("//part"))
    {
      int pos = 0;

      for (auto& measure : part.node().children())
      {
        int shift = 0;

        for(auto& node : measure.children())
        {
          // Check for position shift
          if(!strcmp(node.name(),"backup"))
          {
            shift -= node.text().as_int();
            continue;
          }
          if(!strcmp(node.name(),"forward"))
          {
            shift += node.text().as_int();
            continue;
          }
          
          if(shift)
          {
            pos += shift;
            shift = 0;
          }

          if(strcmp(node.name(),"note"))
            continue;
          
          // Skip if not has no duration
          int duration = node.child("duration").text().as_int();          
          if(duration == 0)
            continue;

          // Write rest
          if(node.child("rest"))
          {
            Pitch pitch;
            pitch._duration = duration;
            pitch._nodePtr = makeNodePtr(node);
            writeToScore(pitch, pos);
            pos += duration;
            continue;
          }

          // Create new note
          Pitch pitch; 
          if(!pitch.fromNode(node))
            throw "unable to parse Pitch from xml_node";

          // Write note in chord
          if(node.child("chord"))
          {
            // Write to last position and don't increment pos
            auto lastPosition = _score.rbegin()->second;
            writeToScore(pitch, pos);
            continue;
          }

          // Write normal note
          writeToScore(pitch, pos);
          pos += duration;

        } // end of for nodes
      } // end of for measures
    } // end of for parts

  }

  void uniformizeDivisions(xml_document& xml)
  {
    std::map<size_t, int> divisions = {};
    std::map<size_t, int> divisionFactors = {};

    // Find all division nodes
    for(auto& division : xml.select_nodes("//divisions"))
      divisions[division.node().offset_debug()] = division.node().text().as_int();

    int maxDivision = std::max_element(divisions.begin(), divisions.end())->second;
    
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

};
