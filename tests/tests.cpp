#include "gtest/gtest.h"
#include "dryad/src/dryad.h"
#include "dryad/src/constants.h"
#include <algorithm>
#include <cstdio>

using namespace Dryad;

class Tests : public ::testing::Test
{
};

class TestNode : public Node
{
public:
    DRYAD_NODE_CLASS_ID(TestNode);

    TestNode(int value = 0)
        : value(value)
    {
    }

    int value;
};

class TestNode2 : public Node
{
public:
    DRYAD_NODE_CLASS_ID(TestNode2);

    TestNode2(int value = 0)
        : value(value)
    {
    }

    int value;
};


TEST(Graph, InsertNode)
{
    int test_value = 42;

    Graph graph;

    graph.insert(new TestNode(test_value));
    graph.insert(new TestNode(test_value));

    EXPECT_EQ(graph.size(), 2) << "Graph contains the wrong count of elements";

    for (Node* node : graph)
    {
        if (TestNode* content = node->get<TestNode>())
            EXPECT_EQ(content->value, test_value);
        else
            FAIL() << "Unable to cast node to underlying type";
    }
}

TEST(Graph, CreateNode)
{
    int test_value = 42;

    Graph graph;
    graph.create<TestNode>(test_value);
    graph.create<TestNode>(test_value);

    EXPECT_EQ(graph.size(), 2) << "Graph contains the wrong count of elements";

    for (Node* node : graph)
    {
        if (TestNode* content = node->get<TestNode>())
            EXPECT_EQ(content->value, test_value);
        else
            FAIL() << "Unable to cast node to underlying type";
    }
}

TEST(Graph, DestroyNode)
{
    Graph graph;
    int test_value = 42;
    TestNode* node_ptr = new TestNode(test_value);
    graph.insert(node_ptr);

    EXPECT_EQ(graph.size(), 1) << "Graph contains the wrong count of elements";

    for (Node* node : graph)
    {
        if (TestNode* content = node->get<TestNode>())
            EXPECT_EQ(content->value, test_value);
        else
            FAIL() << "Unable to cast node to underlying type";
    }

    EXPECT_TRUE(graph.destroy(node_ptr)) << "Error while trying to remove a node from Graph.";
    EXPECT_TRUE(graph.empty()) << "Graph should be empty.";
}

TEST(Graph, LinkNodes)
{
    Graph graph;
    TestNode* node1 = new TestNode(1);
    TestNode* node2 = new TestNode(2);
    TestNode* node3 = new TestNode(3);

    EXPECT_FALSE(graph.link(node1, node2)) << "Nodes not in the same graph should not be able to link.";

    graph.insert(node1);
    graph.insert(node2);
    graph.insert(node3);
    EXPECT_EQ(graph.size(), 3) << "Graph contains the wrong count of elements";

    EXPECT_TRUE(graph.link(node1, node2));
    EXPECT_TRUE(graph.link(node1, node3));

    EXPECT_EQ(node1->edges.size(), 2);
    EXPECT_EQ(node2->edges.size(), 1);
    EXPECT_EQ(node3->edges.size(), 1);

    EXPECT_TRUE(graph.destroy(node1));

    EXPECT_EQ(node1->edges.size(), 0) << "Removed source node edges should have been cleared.";
    EXPECT_EQ(node2->edges.size(), 0) << "Edge node should not reference source node anymore.";
    EXPECT_EQ(node3->edges.size(), 0) << "Edge node should not reference source node anymore.";
    EXPECT_EQ(graph.size(), 2);
}

TEST(Node, ForEachEdge)
{
    TestNode node;

    int test_value = 42;
    TestNode node1(test_value);
    TestNode node2(test_value);
    TestNode2 node3(test_value);

    node.edges.push_back(&node1);
    node.edges.push_back(&node2);
    node.edges.push_back(&node3);

    int result = 0;
    node.forEachEdge<TestNode>([&](TestNode* n)
        {
            result += n->value;
        });

    EXPECT_EQ(result, test_value * 2) << "Only and all nodes of selected type should have been processed.";
}

TEST(Voice, SetCompareById)
{
    Set<Voice*, Voice::CompareByID> set;
    Voice a(3, "3");
    Voice b(1, "1");
    Voice c(2, "2");

    set.insert(&a);
    set.insert(&b);
    set.insert(&c);

    Voice dummy(2, "0");

    auto it = set.find(&dummy);
    EXPECT_NE(it, set.end());

    Voice* voice_found = *it;

    EXPECT_EQ(voice_found, &c);
}

TEST(ScoreFrame, SetCompareByPosition)
{
    Set<ScoreFrame*, ScoreFrame::CompareByPosition> set;
    ScoreFrame a(3);
    ScoreFrame b(1);
    ScoreFrame c(2);

    set.insert(&a);
    set.insert(&b);
    set.insert(&c);

    ScoreFrame dummy(2);

    auto it = set.find(&dummy);
    EXPECT_NE(it, set.end());

    ScoreFrame* frame_found = *it;

    EXPECT_EQ(frame_found, &c);
}

TEST(Score, FindFrameAtPosition)
{
    //FAIL() << "Test not implemented.";
}

TEST(Score, FindLastCommittedFrame)
{
    //FAIL() << "Test not implemented.";
}

TEST(Score, MVP)
{
    // Add voice to score
    Score score;

    int id = 1;
    String name = "test_voice";
    Voice* voice = score.addVoice(id, name);
    EXPECT_NE(voice, nullptr);

    EXPECT_EQ(voice->id, id);

    // Add motif to voice
    Motif* motif = score.create<Motif>();
    EXPECT_NE(motif, nullptr);

    motif->harmonicAnchor = HarmonicAnchor::Chord;
    motif->rhythmicAnchor = AnchorRhythmic::AnyBeat;

    Time duration = Eighth;
    Time offset = 0;
    NoteRelative value = 0;
    MotifNote* motifNote = motif->addNote(value, duration, offset);

    Error error = voice->addMotif(motif);
    EXPECT_EQ(error, Success);

    // Set score scale
    Scale* scale = score.create<Scale>(ScaleLibrary::MajorScale);
    EXPECT_NE(scale, nullptr);

    score.setCurrentScale(scale);

    // Set score progression
    Progression* progression = score.create<Progression>();
    EXPECT_NE(progression, nullptr);

    ProgressionChord* chord1 = score.create<ProgressionChord>(Chord(Degree::Tonic), Whole);
    EXPECT_NE(chord1, nullptr);
    ProgressionChord* chord2 = score.create<ProgressionChord>(Chord(Degree::Dominant), Whole);
    EXPECT_NE(chord2, nullptr);

    progression->nodes.push_back(chord1);
    progression->nodes.push_back(chord2);
    progression->entryNode = chord1;
    chord1->next = chord2;

    score.setCurrentProgression(progression);

    // Commit score duration
    error = score.commitDuration(4 * Whole);
    EXPECT_EQ(error, Success) << "error: " << ToString(error);

    // Dump score
    SerializedScore serializedScore;
    error = score.dump(serializedScore);
    //EXPECT_EQ(error, dryad_no_error);
}

TEST(Motif, SerializeDeserialize)
{
    Session session;
    Score& score = session.getScore();

    Motif* motif = score.create<Motif>();
    ASSERT_NE(motif, nullptr);

    motif->harmonicAnchor = HarmonicAnchor::Scale;
    motif->rhythmicAnchor = AnchorRhythmic::StrongBeat;
    motif->noteIntervalType = NoteIntervalType::Diatonic;
    motif->duration = Quarter + Eighth;

    ASSERT_NE(motif->addNote(0, Eighth, 0), nullptr);
    ASSERT_NE(motif->addNote(2, Eighth, Eighth), nullptr);
    ASSERT_NE(motif->addNote(4, Quarter, Quarter), nullptr);

    SerializedMotif serialized;
    Error error = session.serializeMotif(*motif, serialized);
    EXPECT_EQ(error, Success);

    Motif* roundTrip = nullptr;
    error = session.deserializeMotif(serialized, roundTrip);
    EXPECT_EQ(error, Success);
    ASSERT_NE(roundTrip, nullptr);

    EXPECT_EQ(roundTrip->harmonicAnchor, motif->harmonicAnchor);
    EXPECT_EQ(roundTrip->rhythmicAnchor, motif->rhythmicAnchor);
    EXPECT_EQ(roundTrip->noteIntervalType, motif->noteIntervalType);
    EXPECT_EQ(roundTrip->duration, motif->duration);

    auto collectNotes = [](Motif* source)
    {
        Vector<SerializedMotifNote> notes;
        source->forEachEdge<MotifNote>([&](MotifNote* note)
            {
                SerializedMotifNote serializedNote;
                serializedNote.relativeValue = note->relativeValue;
                serializedNote.duration = note->duration;
                serializedNote.offset = note->offset;
                notes.push_back(serializedNote);
            });

        std::sort(notes.begin(), notes.end(), [](const SerializedMotifNote& a, const SerializedMotifNote& b)
            {
                if (a.offset != b.offset)
                    return a.offset < b.offset;
                if (a.duration != b.duration)
                    return a.duration < b.duration;
                return a.relativeValue < b.relativeValue;
            });
        return notes;
    };

    Vector<SerializedMotifNote> originalNotes = collectNotes(motif);
    Vector<SerializedMotifNote> roundTripNotes = collectNotes(roundTrip);
    ASSERT_EQ(originalNotes.size(), roundTripNotes.size());

    for (size_t i = 0; i < originalNotes.size(); ++i)
    {
        EXPECT_EQ(originalNotes[i].relativeValue, roundTripNotes[i].relativeValue);
        EXPECT_EQ(originalNotes[i].duration, roundTripNotes[i].duration);
        EXPECT_EQ(originalNotes[i].offset, roundTripNotes[i].offset);
    }
}

TEST(GraphFile, SaveLoad)
{
    Session session;
    Score& score = session.getScore();

    score.setCurrentRoot(C);
    Scale* scale = score.create<Scale>(ScaleLibrary::MajorScale);
    ASSERT_NE(scale, nullptr);
    score.setCurrentScale(scale);

    Progression* progressionA = session.createProgression();
    Progression* progressionB = session.createProgression();
    ASSERT_NE(progressionA, nullptr);
    ASSERT_NE(progressionB, nullptr);

    ProgressionChord* chordA1 = session.addChord(*progressionA, Chord(Degree::Tonic), Whole);
    ProgressionChord* chordA2 = session.addChord(*progressionA, Chord(Degree::Dominant), Half);
    ASSERT_NE(chordA1, nullptr);
    ASSERT_NE(chordA2, nullptr);
    session.setEntry(*progressionA, chordA1);
    session.link(chordA1, chordA2);

    ProgressionChord* chordB1 = session.addChord(*progressionB, Chord(Degree::Subdominant), Half);
    ProgressionChord* chordB2 = session.addChord(*progressionB, Chord(Degree::Tonic), Whole);
    ASSERT_NE(chordB1, nullptr);
    ASSERT_NE(chordB2, nullptr);
    session.setEntry(*progressionB, chordB1);
    session.link(chordB1, chordB2);

    ProgressionEvent* eventNode = session.addEvent(*progressionA);
    ASSERT_NE(eventNode, nullptr);
    session.link(chordA2, eventNode);
    eventNode->progressionChange = progressionB;
    eventNode->scaleChange = score.create<Scale>(ScaleLibrary::MinorNaturalScale);

    session.setActiveProgression(progressionB);

    Motif* motifA = session.createMotif();
    Motif* motifB = session.createMotif();
    ASSERT_NE(motifA, nullptr);
    ASSERT_NE(motifB, nullptr);
    motifA->harmonicAnchor = HarmonicAnchor::Chord;
    motifA->rhythmicAnchor = AnchorRhythmic::AnyBeat;
    motifA->noteIntervalType = NoteIntervalType::Diatonic;
    motifA->duration = Quarter;
    ASSERT_NE(motifA->addNote(0, Eighth, 0), nullptr);
    ASSERT_NE(motifA->addNote(2, Eighth, Eighth), nullptr);

    motifB->harmonicAnchor = HarmonicAnchor::Scale;
    motifB->rhythmicAnchor = AnchorRhythmic::StrongBeat;
    motifB->noteIntervalType = NoteIntervalType::Chromatic;
    motifB->duration = Half;
    ASSERT_NE(motifB->addNote(1, Quarter, 0), nullptr);

    Voice* voiceA = session.createVoice(1, "Lead");
    Voice* voiceB = session.createVoice(2, "Bass");
    ASSERT_NE(voiceA, nullptr);
    ASSERT_NE(voiceB, nullptr);
    ASSERT_EQ(voiceA->addMotif(motifA), Success);
    ASSERT_EQ(voiceB->addMotif(motifB), Success);

    eventNode->motifChanges.push_back({motifA, motifB});
    eventNode->voiceChanges.push_back({voiceA, voiceB});

    SerializedGraph original;
    Error error = session.serializeGraph(original);
    ASSERT_EQ(error, Success);

    String path = "test_graph.dryad";
    error = session.saveGraphToFile(path);
    ASSERT_EQ(error, Success);

    Session loaded;
    error = loaded.loadGraphFromFile(path);
    ASSERT_EQ(error, Success);

    SerializedGraph roundTrip;
    error = loaded.serializeGraph(roundTrip);
    ASSERT_EQ(error, Success);

    std::remove(path.c_str());

    ASSERT_EQ(roundTrip.progressions.size(), original.progressions.size());
    EXPECT_EQ(roundTrip.activeProgressionIndex, original.activeProgressionIndex);
    EXPECT_EQ(roundTrip.currentRoot, original.currentRoot);
    EXPECT_EQ(roundTrip.hasScale, original.hasScale);
    ASSERT_EQ(roundTrip.motifs.size(), original.motifs.size());
    EXPECT_EQ(session.getMotifs().size(), original.motifs.size());

    for (size_t progressionIndex = 0; progressionIndex < original.progressions.size(); ++progressionIndex)
    {
        const SerializedProgression& aProgression = original.progressions[progressionIndex];
        const SerializedProgression& bProgression = roundTrip.progressions[progressionIndex];
        ASSERT_EQ(aProgression.nodes.size(), bProgression.nodes.size());
        EXPECT_EQ(aProgression.entryIndex, bProgression.entryIndex);

        for (size_t nodeIndex = 0; nodeIndex < aProgression.nodes.size(); ++nodeIndex)
        {
            const SerializedProgressionNode& a = aProgression.nodes[nodeIndex];
            const SerializedProgressionNode& b = bProgression.nodes[nodeIndex];
            EXPECT_EQ(a.type, b.type);
            EXPECT_EQ(a.nextIndex, b.nextIndex);
            EXPECT_EQ(a.outputs, b.outputs);
            EXPECT_EQ(a.chord.degree, b.chord.degree);
            EXPECT_EQ(a.chord.qualities, b.chord.qualities);
            EXPECT_EQ(a.chord.accidental, b.chord.accidental);
            EXPECT_EQ(a.duration, b.duration);
            EXPECT_EQ(a.scoreEnd, b.scoreEnd);
            EXPECT_EQ(a.progressionChangeIndex, b.progressionChangeIndex);
            EXPECT_EQ(a.hasScaleChange, b.hasScaleChange);
            EXPECT_EQ(a.motifChanges.size(), b.motifChanges.size());
            EXPECT_EQ(a.voiceChanges.size(), b.voiceChanges.size());
        }
    }

    for (size_t i = 0; i < original.motifs.size(); ++i)
    {
        const SerializedMotif& a = original.motifs[i];
        const SerializedMotif& b = roundTrip.motifs[i];
        EXPECT_EQ(a.harmonicAnchor, b.harmonicAnchor);
        EXPECT_EQ(a.rhythmicAnchor, b.rhythmicAnchor);
        EXPECT_EQ(a.noteIntervalType, b.noteIntervalType);
        EXPECT_EQ(a.duration, b.duration);
        ASSERT_EQ(a.notes.size(), b.notes.size());

        for (size_t noteIndex = 0; noteIndex < a.notes.size(); ++noteIndex)
        {
            EXPECT_EQ(a.notes[noteIndex].relativeValue, b.notes[noteIndex].relativeValue);
            EXPECT_EQ(a.notes[noteIndex].duration, b.notes[noteIndex].duration);
            EXPECT_EQ(a.notes[noteIndex].offset, b.notes[noteIndex].offset);
        }
    }

    ASSERT_EQ(roundTrip.voices.size(), original.voices.size());
    for (size_t i = 0; i < original.voices.size(); ++i)
    {
        const SerializedVoiceDefinition& a = original.voices[i];
        const SerializedVoiceDefinition& b = roundTrip.voices[i];
        EXPECT_EQ(a.id, b.id);
        EXPECT_EQ(a.name, b.name);
        EXPECT_EQ(a.motifIndices, b.motifIndices);
    }
}

TEST(ScoreFrame, ChordAnchorUsesScaleDegrees)
{
    Score score;

    Motif* motif = score.create<Motif>();
    motif->harmonicAnchor = HarmonicAnchor::Chord;
    MotifNote* motifNote = motif->addNote(1, Eighth, 0);

    Scale* scale = score.create<Scale>(ScaleLibrary::MajorScale);
    score.setCurrentScale(scale);

    Progression* progression = score.create<Progression>();
    ProgressionChord* chord = score.create<ProgressionChord>(Chord(Degree::Mediant), Whole);
    progression->nodes.push_back(chord);
    progression->entryNode = chord;
    progression->currentProgressionChord = chord;
    score.setCurrentProgression(progression);

    ScoreFrame* frame = score.getOrCreateFrame(0);
    Error err = frame->addMotifNote(motifNote);
    EXPECT_EQ(err, Success);

    NoteInstance* instance = motifNote->findFirstEdge<NoteInstance>();
    ASSERT_NE(instance, nullptr);
    EXPECT_EQ(instance->value, notes[F][MiddleOctave]);
}



// Most additions to the score should have an .add_<something> method,
// so it's added to the graph but also to the cache vectors (voices, motifs, frames, progressions)

TEST(Scale, ChordOffsetsDefaultDegreeQuality)
{
    Vector<NoteRelative> offsets;
    Chord chord(Degree::Tonic);

    Error error = getChordOffsetsFromRoot(chord, &ScaleLibrary::MajorScale, offsets);
    EXPECT_EQ(error, Success);

    Vector<NoteRelative> expected{0, 4, 7, 11};
    EXPECT_EQ(offsets, expected);
}

TEST(Scale, ChordOffsetsOverrideBaseQuality)
{
    Vector<NoteRelative> offsets;
    Chord chord(Degree::Tonic, ChordQuality::Minor);

    Error error = getChordOffsetsFromRoot(chord, &ScaleLibrary::MajorScale, offsets);
    EXPECT_EQ(error, Success);

    Vector<NoteRelative> expected{0, 3, 7, 11};
    EXPECT_EQ(offsets, expected);
}

TEST(Scale, ChordOffsetsExtensionsAlterationsAndAccidental)
{
    Vector<NoteRelative> offsets;
    Chord chord
    (
        Degree::Dominant,
        ChordQuality::Add9 | ChordQuality::Flat5,
        Accidental::Sharp
    );

    Error error = getChordOffsetsFromRoot(chord, &ScaleLibrary::MajorScale, offsets);
    EXPECT_EQ(error, Success);

    Vector<NoteRelative> expected{8, 12, 14, 18, 22};
    EXPECT_EQ(offsets, expected);
}
