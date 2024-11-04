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

    TestElement(int value)
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

    for (SharedPtr<ScoreNode>& node : graph)
    {
        EXPECT_EQ(SharedPtrCast<TestElement>(node)->value, testValue);
    }
}

TEST(ScoreGraph, RemoveNode)
{
    FAIL();
}

TEST(ScoreGraph, LinkNodes)
{
    FAIL();
}

