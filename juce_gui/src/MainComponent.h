#pragma once

#include "NodeGraphComponent.h"
#include "PianoRollComponent.h"
#include "TransportComponent.h"
#include <JuceHeader.h>
#include <dryad/dryad.h>

class MainComponent : public juce::AudioAppComponent {
public:
  MainComponent();
  ~MainComponent() override;

  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
  void
  getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;
  void releaseResources() override;

  void paint(juce::Graphics &g) override;
  void resized() override;

private:
  juce::Synthesiser synthesiser;

  NodeGraphComponent nodeGraph;
  PianoRollComponent pianoRoll;
  TransportComponent transport;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
