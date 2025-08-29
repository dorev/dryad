#pragma once

#include <JuceHeader.h>
#include <dryad/dryad.h>
#include <map>
#include <optional>

class NodeGraphComponent;
class PianoRollComponent;

class DryadInterface {
public:
  DryadInterface();

  void rebuild(const NodeGraphComponent &graph, const PianoRollComponent &roll);

  std::vector<int> getMotifNotesForStep(int step) const;
  std::optional<int> getProgressionRootForStep(int step) const;

private:
  Dryad::Graph graph;
  Dryad::Scale scale;
  Dryad::Motif *motif{nullptr};
  std::vector<Dryad::ProgressionChord *> progressionChords;
  static constexpr int stepsPerChord = 4;
  static constexpr int baseMidi = 60; // C4
};

