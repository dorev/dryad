#pragma once

#include <JuceHeader.h>

class NodeGraphComponent;
class PianoRollComponent;
class DryadInterface;

class TransportComponent : public juce::Component, private juce::Timer {
public:
  TransportComponent(NodeGraphComponent &graph, PianoRollComponent &roll,
                     juce::Synthesiser &synth, DryadInterface &dryad);
  ~TransportComponent() override = default;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  void play();
  void stop();
  void timerCallback() override;

  NodeGraphComponent &nodeGraph;
  PianoRollComponent &pianoRoll;
  juce::Synthesiser &synthesiser;
  DryadInterface &dryad;

  juce::TextButton playButton{"Play"};
  juce::TextButton stopButton{"Stop"};

  int step = 0;
};
