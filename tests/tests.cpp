#include "gtest/gtest.h"
#include "dryad/src/dryad.h"

using namespace Dryad;

class Tests : public ::testing::Test
{
};

class test_node : public Node
{
public:
    DRYAD_NODE_CLASS_ID(test_node);

    test_node(int value = 0)
        : value(value)
    {
    }

    int value;
};

class test_node2 : public Node
{
public:
    DRYAD_NODE_CLASS_ID(test_node2);

    test_node2(int value = 0)
        : value(value)
    {
    }

    int value;
};


TEST(drayd_graph, insert_node)
{
    int test_value = 42;

    Graph graph;

    graph.insert(new test_node(test_value));
    graph.insert(new test_node(test_value));

    EXPECT_EQ(graph.size(), 2) << "Graph contains the wrong count of elements";

    for (Node* node : graph)
    {
        if (test_node* content = node->get<test_node>())
            EXPECT_EQ(content->value, test_value);
        else
            FAIL() << "Unable to cast node to underlying type";
    }
}

TEST(drayd_graph, create_node)
{
    int test_value = 42;

    Graph graph;
    graph.create<test_node>(test_value);
    graph.create<test_node>(test_value);

    EXPECT_EQ(graph.size(), 2) << "Graph contains the wrong count of elements";

    for (Node* node : graph)
    {
        if (test_node* content = node->get<test_node>())
            EXPECT_EQ(content->value, test_value);
        else
            FAIL() << "Unable to cast node to underlying type";
    }
}

TEST(drayd_graph, destroy_node)
{
    Graph graph;
    int test_value = 42;
    test_node* node_ptr = new test_node(test_value);
    graph.insert(node_ptr);

    EXPECT_EQ(graph.size(), 1) << "Graph contains the wrong count of elements";

    for (Node* node : graph)
    {
        if (test_node* content = node->get<test_node>())
            EXPECT_EQ(content->value, test_value);
        else
            FAIL() << "Unable to cast node to underlying type";
    }

    EXPECT_TRUE(graph.destroy(node_ptr)) << "Error while trying to remove a node from Graph.";
    EXPECT_TRUE(graph.empty()) << "Graph should be empty.";
}

TEST(drayd_graph, link)
{
    Graph graph;
    test_node* node1 = new test_node(1);
    test_node* node2 = new test_node(2);
    test_node* node3 = new test_node(3);

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

TEST(Node, forEachEdge)
{
    test_node node;

    int test_value = 42;
    test_node node1(test_value);
    test_node node2(test_value);
    test_node2 node3(test_value);

    node.edges.push_back(&node1);
    node.edges.push_back(&node2);
    node.edges.push_back(&node3);

    int result = 0;
    node.forEachEdge<test_node>([&](test_node* n)
        {
            result += n->value;
        });

    EXPECT_EQ(result, test_value * 2) << "Only and all nodes of selected type should have been processed.";
}

TEST(Voice, set_compare_by_id)
{
    Set<Voice*, Voice::compareByID> set;
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

TEST(ScoreFrame, set_compare_by_position)
{
    Set<ScoreFrame*, ScoreFrame::compare_by_position> set;
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

TEST(Score, findFrameAtPosition)
{
    //FAIL() << "Test not implemented.";
}

TEST(Score, findLastCommittedFrame)
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

    motif->harmonicAnchor = HarmonicAnchor::chord;
    motif->rhythmicAnchor = AnchorRhythmic::any_beat;

    Time duration = eighth;
    Time relativePosition = 0;
    NoteRelative value = 0;
    MotifNote* motifNote = motif->addNote(value, duration, relativePosition);

    Error error = voice->addMotif(motif);
    EXPECT_EQ(error, Success);

    // Set score scale
    Scale* scale = score.create<Scale>(ScaleLibrary::major_scale);
    EXPECT_NE(scale, nullptr);

    score.currentScale = scale;

    // Set score progression
    Progression* progression = score.create<Progression>();
    EXPECT_NE(progression, nullptr);

    ProgressionChord* chord1 = score.create<ProgressionChord>(Chord(Degree::tonic), whole);
    EXPECT_NE(chord1, nullptr);
    ProgressionChord* chord2 = score.create<ProgressionChord>(Chord(Degree::dominant), whole);
    EXPECT_NE(chord2, nullptr);

    progression->nodes.push_back(chord1);
    progression->nodes.push_back(chord2);
    progression->entryNode = chord1;
    chord1->next = chord2;

    score.currentProgression = progression;

    // Commit score duration
    error = score.commit(4 * whole);
    EXPECT_EQ(error, Success) << "error: " << ToString(error);

    // Dump score
    SerializedScore serializedScore;
    error = score.dump(serializedScore);
    //EXPECT_EQ(error, dryad_no_error);
}



// Most additions to the score should have an .add_<something> method,
// so it's added to the graph but also to the cache vectors (voices, motifs, frames, progressions)

TEST(Scale, chord_offsets_default_degree_quality)
{
    Vector<NoteRelative> offsets;
    Chord chord(Degree::tonic);

    Error error = getChordOffsetsFromRoot(chord, &ScaleLibrary::major_scale, offsets);
    EXPECT_EQ(error, Success);

    Vector<NoteRelative> expected{0, 4, 7, 11};
    EXPECT_EQ(offsets, expected);
}

TEST(Scale, chord_offsets_override_base_quality)
{
    Vector<NoteRelative> offsets;
    Chord chord(Degree::tonic, ChordQuality::minor);

    Error error = getChordOffsetsFromRoot(chord, &ScaleLibrary::major_scale, offsets);
    EXPECT_EQ(error, Success);

    Vector<NoteRelative> expected{0, 3, 7, 11};
    EXPECT_EQ(offsets, expected);
}

TEST(Scale, chord_offsets_extensions_alterations_and_accidental)
{
    Vector<NoteRelative> offsets;
    Chord chord
    (
        Degree::dominant,
        ChordQuality::add9 | ChordQuality::flat5,
        Accidental::sharp
    );

    Error error = getChordOffsetsFromRoot(chord, &ScaleLibrary::major_scale, offsets);
    EXPECT_EQ(error, Success);

    Vector<NoteRelative> expected{8, 12, 14, 18, 22};
    EXPECT_EQ(offsets, expected);
}
