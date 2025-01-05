#include "gtest/gtest.h"
#include "dryad/src/dryad.h"

class Tests : public ::testing::Test
{
};

class test_node : public dryad_node
{
public:
    DRYAD_CLASS_ID(test_node);

    test_node(int value = 0)
        : value(value)
    {
    }

    int value;
};

class test_node2 : public dryad_node
{
public:
    DRYAD_CLASS_ID(test_node2);

    test_node2(int value = 0)
        : value(value)
    {
    }

    int value;
};


TEST(drayd_graph, insert_node)
{
    int test_value = 42;

    dryad_graph graph;

    graph.insert(new test_node(test_value));
    graph.insert(new test_node(test_value));

    EXPECT_EQ(graph.size(), 2) << "Graph contains the wrong count of elements";

    for (dryad_node* node : graph)
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

    dryad_graph graph;
    graph.create<test_node>(test_value);
    graph.create<test_node>(test_value);

    EXPECT_EQ(graph.size(), 2) << "Graph contains the wrong count of elements";

    for (dryad_node* node : graph)
    {
        if (test_node* content = node->get<test_node>())
            EXPECT_EQ(content->value, test_value);
        else
            FAIL() << "Unable to cast node to underlying type";
    }
}

TEST(drayd_graph, destroy_node)
{
    dryad_graph graph;
    int test_value = 42;
    test_node* node_ptr = new test_node(test_value);
    graph.insert(node_ptr);

    EXPECT_EQ(graph.size(), 1) << "Graph contains the wrong count of elements";

    for (dryad_node* node : graph)
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
    dryad_graph graph;
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

TEST(dryad_node, for_each)
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
    node.for_each<test_node>([&](test_node* n)
        {
            result += n->value;
        });

    EXPECT_EQ(result, test_value * 2) << "Only and all nodes of selected type should have been processed.";
}

TEST(dryad_score, MVP)
{
    // Add voice to score
    dryad_score score;

    dryad_voice* voice = score.create<dryad_voice>();
    EXPECT_NE(voice, nullptr);
    voice->id = 2;

    score.for_each<dryad_voice>([](dryad_voice* v)
        {
            EXPECT_EQ(v->id, 2);
        });

    // Add motif to voice
    dryad_motif* motif = score.create<dryad_motif>();
    EXPECT_NE(motif, nullptr);

    motif->harmonic_anchor = dryad_harmonic_anchor::chord;
    motif->rhythmic_anchor = dryad_rhythmic_anchor::any_beat;

    dryad_time duration = eighth;
    dryad_time position = 0;
    dryad_note_relative value = 0;
    dryad_motif_note* motif_note = motif->add_note(value, duration, position);

    bool link_result = score.link(voice, motif);
    EXPECT_EQ(link_result, true);

    // Set score scale
    dryad_scale* scale = score.create<dryad_scale>(dryad_scale_library::major_scale);
    EXPECT_NE(scale, nullptr);

    score.current_scale = scale;

    // Set score progression
    dryad_progression* progression = score.create<dryad_progression>();
    EXPECT_NE(progression, nullptr);

    dryad_progression_chord* chord1 = score.create<dryad_progression_chord>(dryad_chord(dryad_degree::tonic), whole);
    EXPECT_NE(chord1, nullptr);
    dryad_progression_chord* chord2 = score.create<dryad_progression_chord>(dryad_chord(dryad_degree::dominant), whole);
    EXPECT_NE(chord2, nullptr);

    progression->nodes.push_back(chord1);
    progression->nodes.push_back(chord2);
    progression->entry_node = chord1;
    chord1->next = chord2;

    score.current_progression = progression;

    // Commit score duration
    dryad_error error = score.commit(4 * whole);
    EXPECT_EQ(error, dryad_no_error);

    // Dump score
    dryad_serialized_score serialized_score;
    error = score.dump(serialized_score);
    //EXPECT_EQ(error, dryad_no_error);
}



// Most additions to the score should have an .add_<something> method,
// so it's added to the graph but also to the 'shortcut' vectors (voices, motifs, frames, progressions)
