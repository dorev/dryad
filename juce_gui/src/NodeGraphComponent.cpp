#include "NodeGraphComponent.h"

namespace {
const juce::StringArray chordOptions{"C", "Dm", "Em", "F", "G", "Am", "Bdim"};
constexpr int nodeRadius = 20;
} // namespace

NodeGraphComponent::NodeGraphComponent() { setWantsKeyboardFocus(true); }

void NodeGraphComponent::paint(juce::Graphics &g) {
  g.fillAll(juce::Colours::darkgrey);

  for (size_t i = 0; i < nodes.size(); ++i) {
    auto &node = nodes[i];
    auto bounds = juce::Rectangle<int>(node.position.x - nodeRadius,
                                       node.position.y - nodeRadius,
                                       nodeRadius * 2, nodeRadius * 2);

    g.setColour(juce::Colours::lightblue);
    g.fillEllipse(bounds.toFloat());
    g.setColour(juce::Colours::black);
    g.drawEllipse(bounds.toFloat(), 2.0f);
    g.drawText(chordName(node.chordIndex), bounds,
               juce::Justification::centred);

    if (i > 0) {
      auto &prev = nodes[i - 1];
      g.drawLine(prev.position.x, prev.position.y, node.position.x,
                 node.position.y, 2.0f);
    }
  }
}

int NodeGraphComponent::hitTestNode(juce::Point<int> pos) const {
  for (size_t i = 0; i < nodes.size(); ++i) {
    if (pos.getDistanceFrom(nodes[i].position) <= nodeRadius)
      return static_cast<int>(i);
  }
  return -1;
}

void NodeGraphComponent::mouseDown(const juce::MouseEvent &e) {
  if (e.mods.isRightButtonDown()) {
    int idx = hitTestNode(e.getPosition());
    if (idx >= 0) {
      nodes[idx].chordIndex = (nodes[idx].chordIndex + 1) % chordOptions.size();
      repaint();
    }
    return;
  }

  int idx = hitTestNode(e.getPosition());
  if (idx >= 0) {
    draggingIndex = idx;
  } else {
    nodes.push_back({e.getPosition(), 0});
    draggingIndex = static_cast<int>(nodes.size()) - 1;
    repaint();
  }
}

void NodeGraphComponent::mouseDrag(const juce::MouseEvent &e) {
  if (draggingIndex >= 0) {
    nodes[draggingIndex].position = e.getPosition();
    repaint();
  }
}

std::vector<juce::String> NodeGraphComponent::getProgression() const {
  std::vector<juce::String> prog;
  for (auto &n : nodes)
    prog.push_back(chordName(n.chordIndex));
  return prog;
}

juce::String NodeGraphComponent::chordName(int index) const {
  return chordOptions[index % chordOptions.size()];
}
