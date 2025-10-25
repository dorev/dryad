#include "MainComponent.h"

using namespace Dryad;

namespace {
struct SineWaveSound : public juce::SynthesiserSound {
  bool appliesToNote(int) override { return true; }
  bool appliesToChannel(int) override { return true; }
};

struct SineWaveVoice : public juce::SynthesiserVoice {
  double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0;

  bool canPlaySound(juce::SynthesiserSound *s) override {
    return dynamic_cast<SineWaveSound *>(s) != nullptr;
  }

  void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *,
                 int) override {
    currentAngle = 0.0;
    level = velocity;
    tailOff = 0.0;
    auto cyclesPerSecond =
        juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    auto cyclesPerSample = cyclesPerSecond / getSampleRate();
    angleDelta = cyclesPerSample * juce::MathConstants<double>::twoPi;
  }

  void stopNote(float, bool allowTailOff) override {
    if (allowTailOff) {
      if (tailOff == 0.0)
        tailOff = 1.0;
    } else {
      clearCurrentNote();
      angleDelta = 0.0;
    }
  }

  void pitchWheelMoved(int) override {}
  void controllerMoved(int, int) override {}

  void renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample,
                       int numSamples) override {
    if (angleDelta != 0.0) {
      if (tailOff > 0.0) {
        while (--numSamples >= 0) {
          auto currentSample =
              (float)(std::sin(currentAngle) * level * tailOff);
          for (int chan = 0; chan < outputBuffer.getNumChannels(); ++chan)
            outputBuffer.addSample(chan, startSample, currentSample);

          currentAngle += angleDelta;
          ++startSample;

          tailOff *= 0.99;
          if (tailOff <= 0.005) {
            clearCurrentNote();
            angleDelta = 0.0;
            break;
          }
        }
      } else {
        while (--numSamples >= 0) {
          auto currentSample = (float)(std::sin(currentAngle) * level);
          for (int chan = 0; chan < outputBuffer.getNumChannels(); ++chan)
            outputBuffer.addSample(chan, startSample, currentSample);

          currentAngle += angleDelta;
          ++startSample;
        }
      }
    }
  }
};
} // namespace

MainComponent::MainComponent()
    : transport(nodeGraph, pianoRoll, synthesiser, dryadInterface) {
  for (int i = 0; i < 8; ++i)
    synthesiser.addVoice(new SineWaveVoice());
  synthesiser.addSound(new SineWaveSound());

  addAndMakeVisible(nodeGraph);
  addAndMakeVisible(pianoRoll);
  addAndMakeVisible(transport);

  setSize(800, 600);
  setAudioChannels(0, 2);
}

MainComponent::~MainComponent() { shutdownAudio(); }

void MainComponent::prepareToPlay(int, double sampleRate) {
  synthesiser.setCurrentPlaybackSampleRate(sampleRate);
}

void MainComponent::getNextAudioBlock(
    const juce::AudioSourceChannelInfo &bufferToFill) {
  bufferToFill.clearActiveBufferRegion();
  synthesiser.renderNextBlock(*bufferToFill.buffer, bufferToFill.startSample,
                              bufferToFill.numSamples);
}

void MainComponent::releaseResources() {}

void MainComponent::paint(juce::Graphics &g) {
  g.fillAll(juce::Colours::darkgrey);
}

void MainComponent::resized() {
  auto area = getLocalBounds();
  auto transportHeight = 40;
  transport.setBounds(area.removeFromBottom(transportHeight));
  auto graphHeight = area.getHeight() / 3;
  nodeGraph.setBounds(area.removeFromTop(graphHeight));
  pianoRoll.setBounds(area);
}
