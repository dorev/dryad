#include "gtest/gtest.h"
#include "dryad/src/dryad.h"
#include "dryad/src/constants.h"

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

    for (Voice* v : score.cachedVoices)
        EXPECT_EQ(v->id, id);

    // Add motif to voice
    Motif* motif = score.create<Motif>();
    EXPECT_NE(motif, nullptr);

    motif->harmonicAnchor = HarmonicAnchor::Chord;
    motif->rhythmicAnchor = AnchorRhythmic::AnyBeat;

    Time duration = Eighth;
    Time relativePosition = 0;
    NoteRelative value = 0;
    MotifNote* motifNote = motif->addNote(value, duration, relativePosition);

    Error error = voice->addMotif(motif);
    EXPECT_EQ(error, Success);

    // Set score scale
    Scale* scale = score.create<Scale>(ScaleLibrary::MajorScale);
    EXPECT_NE(scale, nullptr);

    score.currentScale = scale;

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

    score.currentProgression = progression;

    // Commit score duration
    error = score.commit(4 * Whole);
    EXPECT_EQ(error, Success) << "error: " << ToString(error);

    // Dump score
    SerializedScore serializedScore;
    error = score.dump(serializedScore);
    //EXPECT_EQ(error, dryad_no_error);
}

TEST(ScoreFrame, ChordAnchorUsesScaleDegrees)
{
    Score score;

    Motif* motif = score.create<Motif>();
    motif->harmonicAnchor = HarmonicAnchor::Chord;
    MotifNote* motifNote = motif->addNote(1, Eighth, 0);

    Scale* scale = score.create<Scale>(ScaleLibrary::MajorScale);
    score.currentScale = scale;

    Progression* progression = score.create<Progression>();
    ProgressionChord* chord = score.create<ProgressionChord>(Chord(Degree::Mediant), Whole);
    progression->nodes.push_back(chord);
    progression->entryNode = chord;
    progression->currentProgressionChord = chord;
    score.currentProgression = progression;

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
