#include "PianoRollComponent.h"

PianoRollComponent::PianoRollComponent() { setWantsKeyboardFocus(true); }

void PianoRollComponent::paint(juce::Graphics &g) {
  g.fillAll(juce::Colours::black);
  auto bounds = getLocalBounds();
  auto cellWidth = bounds.getWidth() / numSteps;
  auto cellHeight = bounds.getHeight() / numNotes;

  g.setColour(juce::Colours::grey);
  for (int i = 0; i <= numSteps; ++i)
    g.drawLine((float)(i * cellWidth), 0.0f, (float)(i * cellWidth),
               (float)bounds.getHeight());
  for (int j = 0; j <= numNotes; ++j)
    g.drawLine(0.0f, (float)(j * cellHeight), (float)bounds.getWidth(),
               (float)(j * cellHeight));

  g.setColour(juce::Colours::lightgreen);
  for (int n = 0; n < numNotes; ++n)
    for (int s = 0; s < numSteps; ++s)
      if (grid[n][s])
        g.fillRect(s * cellWidth, n * cellHeight, cellWidth, cellHeight);
}

void PianoRollComponent::mouseDown(const juce::MouseEvent &e) {
  auto bounds = getLocalBounds();
  auto cellWidth = bounds.getWidth() / numSteps;
  auto cellHeight = bounds.getHeight() / numNotes;
  int step = juce::jlimit(0, numSteps - 1, e.x / cellWidth);
  int note = juce::jlimit(0, numNotes - 1, e.y / cellHeight);
  grid[note][step] = !grid[note][step];
  repaint();
}

std::vector<int> PianoRollComponent::getNotesForStep(int step) const {
  std::vector<int> notes;
  for (int n = 0; n < numNotes; ++n)
    if (grid[n][step])
      notes.push_back(n);
  return notes;
}
