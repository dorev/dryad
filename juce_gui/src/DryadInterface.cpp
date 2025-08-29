#include "DryadInterface.h"
#include "NodeGraphComponent.h"
#include "PianoRollComponent.h"

namespace {
struct ChordMapEntry {
  Dryad::Degree degree;
  Dryad::ChordQuality quality;
};

const std::map<juce::String, ChordMapEntry> chordMap = {
    {"C", {Dryad::Degree::Tonic, Dryad::ChordQuality::Major}},
    {"Dm", {Dryad::Degree::Supertonic, Dryad::ChordQuality::Minor}},
    {"Em", {Dryad::Degree::Mediant, Dryad::ChordQuality::Minor}},
    {"F", {Dryad::Degree::Subdominant, Dryad::ChordQuality::Major}},
    {"G", {Dryad::Degree::Dominant, Dryad::ChordQuality::Major}},
    {"Am", {Dryad::Degree::Submediant, Dryad::ChordQuality::Minor}},
    {"Bdim", {Dryad::Degree::LeadingTone, Dryad::ChordQuality::Diminished}},
};
}

DryadInterface::DryadInterface()
    : scale(Dryad::ScaleLibrary::IonianOffsets,
            Dryad::ScaleLibrary::IonianDegreeQualities) {}

void DryadInterface::rebuild(const NodeGraphComponent &graphComp,
                             const PianoRollComponent &roll) {
  graph = Dryad::Graph{}; // reset graph and owned nodes
  motif = graph.create<Dryad::Motif>();

  int totalSteps = roll.getNumSteps();
  for (int step = 0; step < totalSteps; ++step) {
    auto notes = roll.getNotesForStep(step);
    for (auto n : notes)
      motif->addNote(n, 1, step);
  }
  motif->updateDuration();

  auto progression = graph.create<Dryad::Progression>();
  progressionChords.clear();
  Dryad::ProgressionNode *prev = nullptr;

  for (auto &name : graphComp.getProgression()) {
    auto it = chordMap.find(name);
    if (it == chordMap.end())
      continue;
    Dryad::Chord chord(it->second.degree, it->second.quality,
                       Dryad::Accidental::Natural);
    auto *node = graph.create<Dryad::ProgressionChord>(chord, stepsPerChord);
    if (!progression->entryNode)
      progression->entryNode = node;
    if (prev)
      prev->next = node;
    prev = node;
    progression->nodes.push_back(node);
    progressionChords.push_back(node);
  }
}

std::vector<int> DryadInterface::getMotifNotesForStep(int step) const {
  std::vector<int> result;
  if (!motif)
    return result;

  motif->forEachEdge<Dryad::MotifNote>([&](Dryad::MotifNote *note) {
    if ((int)note->relativePosition == step)
      result.push_back(72 + note->relativeValue);
  });
  return result;
}

std::optional<int> DryadInterface::getProgressionRootForStep(int step) const {
  int chordIdx = step / stepsPerChord;
  if (chordIdx < 0 || chordIdx >= (int)progressionChords.size())
    return std::nullopt;

  auto *chordNode = progressionChords[chordIdx];
  auto degree = chordNode->chord.degree;
  auto accidental = chordNode->chord.accidental;
  int offset = scale.getDegreeNoteOffset(degree);
  return baseMidi + offset + static_cast<int>(accidental);
}

