#pragma once

#include <JuceHeader.h>

class PianoRollComponent : public juce::Component {
public:
  PianoRollComponent();
  ~PianoRollComponent() override = default;

  void paint(juce::Graphics &) override;
  void mouseDown(const juce::MouseEvent &) override;

  std::vector<int> getNotesForStep(int step) const;
  int getNumSteps() const { return numSteps; }

private:
  static constexpr int numNotes = 12; // one octave
  static constexpr int numSteps = 16;
  bool grid[numNotes][numSteps] = {};
};
