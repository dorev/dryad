#include "gtest/gtest.h"
#include "dryad/src/scoregraph.h"
#include "dryad/src/constants.h"

using namespace Dryad;

class Tests : public ::testing::Test
{
};

class TestElement : public ScoreNode
{
public:
    DRYAD_CLASS_ID(TestElement);

    TestElement(int value = 0)
        : value(value)
    {
    }

    int value;
};

TEST(ScoreGraph, CreateNode)
{
    int testValue = 42;

    ScoreGraph graph;
    graph.Insert(MakeShared<TestElement>(testValue));
    graph.Insert(MakeShared<TestElement>(testValue));

    EXPECT_EQ(graph.Size(), 2) << "ScoreGraph contains the wrong count of elements";

    for (SharedPtr<ScoreNode>& node : graph)
    {
        TestElement element;
        if (node->CastTo<TestElement>(element))
        {
            EXPECT_EQ(element.value, testValue);
        }
        else
        {
            FAIL() << "Unable to cast node to underlying type";
        }
    }
}

TEST(ScoreGraph, RemoveNode)
{
    ScoreGraph graph;
    int testValue = 42;
    SharedPtr<TestElement> elementPtr = MakeShared<TestElement>(testValue);
    graph.Insert(elementPtr);

    EXPECT_EQ(graph.Size(), 1) << "ScoreGraph contains the wrong count of elements";

    for (SharedPtr<ScoreNode>& node : graph)
    {
        TestElement element;
        if (node->CastTo<TestElement>(element))
        {
            EXPECT_EQ(element.value, testValue);
        }
        else
        {
            FAIL() << "Unable to cast node to underlying type";
        }
    }

    EXPECT_TRUE(graph.Remove(elementPtr)) << "Error while trying to remove a node from ScoreGraph.";
    EXPECT_TRUE(graph.Empty()) << "ScoreGraph should be empty.";
}

TEST(ScoreGraph, LinkNodes)
{
    ScoreGraph graph;
    SharedPtr<TestElement> element1 = MakeShared<TestElement>(1);
    SharedPtr<TestElement> element2 = MakeShared<TestElement>(2);
    SharedPtr<TestElement> element3 = MakeShared<TestElement>(3);

    EXPECT_FALSE(graph.LinkNodes(element1, element2)) << "Nodes not in the same graph should not be able to link.";

    graph.Insert(element1);
    graph.Insert(element2);
    graph.Insert(element3);
    EXPECT_EQ(graph.Size(), 3) << "ScoreGraph contains the wrong count of elements";

    EXPECT_TRUE(graph.LinkNodes(element1, element2));
    EXPECT_TRUE(graph.LinkNodes(element1, element3));

    EXPECT_EQ(element1->edges.Size(), 2);
    EXPECT_EQ(element2->edges.Size(), 1);
    EXPECT_EQ(element3->edges.Size(), 1);

    EXPECT_TRUE(graph.Remove(element1));

    EXPECT_EQ(element1->edges.Size(), 0) << "Removed source node edges should have been cleared.";
    EXPECT_EQ(element2->edges.Size(), 0) << "Edge node should not reference source node anymore.";
    EXPECT_EQ(element3->edges.Size(), 0) << "Edge node should not reference source node anymore.";
    EXPECT_EQ(graph.Size(), 2);
}

