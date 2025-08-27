#include "TransportComponent.h"
#include "NodeGraphComponent.h"
#include "PianoRollComponent.h"
#include <map>

TransportComponent::TransportComponent(NodeGraphComponent &graph,
                                       PianoRollComponent &roll,
                                       juce::Synthesiser &synth)
    : nodeGraph(graph), pianoRoll(roll), synthesiser(synth) {
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
  step = 0;
  startTimerHz(8); // 120bpm 16th notes
}

void TransportComponent::stop() {
  stopTimer();
  synthesiser.allNotesOff(0);
}

namespace {
int midiNoteFromName(const juce::String &name) {
  static std::map<juce::String, int> map{
      {"C", 60},  {"C#", 61}, {"Dm", 62}, {"D", 62}, {"D#", 63},
      {"Em", 64}, {"F", 65},  {"F#", 66}, {"G", 67}, {"G#", 68},
      {"Am", 69}, {"A", 69},  {"A#", 70}, {"B", 71}, {"Bdim", 71}};
  auto it = map.find(name);
  if (it != map.end())
    return it->second;
  return 60;
}
} // namespace

void TransportComponent::timerCallback() {
  synthesiser.allNotesOff(0);

  auto notes = pianoRoll.getNotesForStep(step);
  for (auto n : notes)
    synthesiser.noteOn(1, 72 + n, 0.8f); // C5 + n

  auto chords = nodeGraph.getProgression();
  if (!chords.empty()) {
    int chordIdx = step / 4;
    if (chordIdx < (int)chords.size()) {
      int root = midiNoteFromName(chords[chordIdx]);
      synthesiser.noteOn(1, root, 0.5f);
    }
  }

  step = (step + 1) % pianoRoll.getNumSteps();
}
