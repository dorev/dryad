#pragma once

#include <JuceHeader.h>

class NodeGraphComponent : public juce::Component {
public:
  NodeGraphComponent();
  ~NodeGraphComponent() override = default;

  void paint(juce::Graphics &) override;
  void mouseDown(const juce::MouseEvent &) override;
  void mouseDrag(const juce::MouseEvent &) override;

  std::vector<juce::String> getProgression() const;

private:
  struct Node {
    juce::Point<int> position;
    int chordIndex = 0;
  };

  int hitTestNode(juce::Point<int> pos) const;

  juce::String chordName(int index) const;

  std::vector<Node> nodes;
  int draggingIndex = -1;
};
