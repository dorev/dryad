#include "gtest/gtest.h"
#include "dryad/src/graph.h"
#include "dryad/src/constants.h"

using namespace Dryad;

class Tests : public ::testing::Test
{
};

class TestElement : public Node
{
public:
    DRYAD_CLASS_ID(TestElement);

    TestElement(int value = 0)
        : value(value)
    {
    }

    int value;
};

TEST(Graph, InsertNode)
{
    int testValue = 42;

    SharedPtr<Graph> graph = MakeShared<Graph>();
    graph->Insert(MakeShared<TestElement>(testValue));
    graph->Insert(MakeShared<TestElement>(testValue));

    EXPECT_EQ(graph->Size(), 2) << "Graph contains the wrong count of elements";

    for (SharedPtr<Node>& node : *graph)
    {
        SharedPtr<TestElement> element;
        if (node->GetElement<TestElement>(element))
        {
            EXPECT_EQ(element->value, testValue);
        }
        else
        {
            FAIL() << "Unable to cast node to underlying type";
        }
    }
}

TEST(Graph, EmplaceNode)
{
    int testValue = 42;

    SharedPtr<Graph> graph = MakeShared<Graph>();
    graph->Emplace<TestElement>(testValue);
    graph->Emplace<TestElement>(testValue);

    EXPECT_EQ(graph->Size(), 2) << "Graph contains the wrong count of elements";

    for (SharedPtr<Node>& node : *graph)
    {
        SharedPtr<TestElement> element;
        if (node->GetElement<TestElement>(element))
        {
            EXPECT_EQ(element->value, testValue);
        }
        else
        {
            FAIL() << "Unable to cast node to underlying type";
        }
    }
}

TEST(Graph, RemoveNode)
{
    SharedPtr<Graph> graph = MakeShared<Graph>();
    int testValue = 42;
    SharedPtr<TestElement> elementPtr = MakeShared<TestElement>(testValue);
    graph->Insert(elementPtr);

    EXPECT_EQ(graph->Size(), 1) << "Graph contains the wrong count of elements";

    for (SharedPtr<Node>& node : *graph)
    {
        SharedPtr<TestElement> element;
        if (node->GetElement<TestElement>(element))
        {
            EXPECT_EQ(element->value, testValue);
        }
        else
        {
            FAIL() << "Unable to cast node to underlying type";
        }
    }

    EXPECT_TRUE(graph->Remove(elementPtr)) << "Error while trying to remove a node from Graph.";
    EXPECT_TRUE(graph->Empty()) << "Graph should be empty.";
}

TEST(Graph, LinkNodes)
{
    SharedPtr<Graph> graph = MakeShared<Graph>();
    SharedPtr<TestElement> element1 = MakeShared<TestElement>(1);
    SharedPtr<TestElement> element2 = MakeShared<TestElement>(2);
    SharedPtr<TestElement> element3 = MakeShared<TestElement>(3);

    EXPECT_FALSE(graph->LinkNodes(element1, element2)) << "Nodes not in the same graph should not be able to link.";

    graph->Insert(element1);
    graph->Insert(element2);
    graph->Insert(element3);
    EXPECT_EQ(graph->Size(), 3) << "Graph contains the wrong count of elements";

    EXPECT_TRUE(graph->LinkNodes(element1, element2));
    EXPECT_TRUE(graph->LinkNodes(element1, element3));

    EXPECT_EQ(element1->edges.Size(), 2);
    EXPECT_EQ(element2->edges.Size(), 1);
    EXPECT_EQ(element3->edges.Size(), 1);

    EXPECT_TRUE(graph->Remove(element1));

    EXPECT_EQ(element1->edges.Size(), 0) << "Removed source node edges should have been cleared.";
    EXPECT_EQ(element2->edges.Size(), 0) << "Edge node should not reference source node anymore.";
    EXPECT_EQ(element3->edges.Size(), 0) << "Edge node should not reference source node anymore.";
    EXPECT_EQ(graph->Size(), 2);
}

