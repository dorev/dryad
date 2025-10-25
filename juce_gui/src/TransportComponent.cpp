#include "TransportComponent.h"
#include "NodeGraphComponent.h"
#include "PianoRollComponent.h"
#include "DryadInterface.h"

TransportComponent::TransportComponent(NodeGraphComponent &graph,
                                       PianoRollComponent &roll,
                                       juce::Synthesiser &synth,
                                       DryadInterface &d)
    : nodeGraph(graph), pianoRoll(roll), synthesiser(synth), dryad(d) {
  addAndMakeVisible(playButton);
  addAndMakeVisible(stopButton);

  playButton.onClick = [this] { play(); };
  stopButton.onClick = [this] { stop(); };
}

void TransportComponent::paint(juce::Graphics &g) {
  g.fillAll(juce::Colours::darkgrey);
}

void TransportComponent::resized() {
  auto area = getLocalBounds().reduced(10);
  playButton.setBounds(area.removeFromLeft(80));
  stopButton.setBounds(area.removeFromLeft(80));
}

void TransportComponent::play() {
  dryad.rebuild(nodeGraph, pianoRoll);
  step = 0;
  startTimerHz(8); // 120bpm 16th notes
}

void TransportComponent::stop() {
  stopTimer();
  synthesiser.allNotesOff(1, true);
}

void TransportComponent::timerCallback() {
  synthesiser.allNotesOff(1, true);
  for (auto midi : dryad.getMotifNotesForStep(step))
    synthesiser.noteOn(1, midi, 0.8f);

  if (auto root = dryad.getProgressionRootForStep(step))
    synthesiser.noteOn(1, *root, 0.5f);

  step = (step + 1) % pianoRoll.getNumSteps();
}
