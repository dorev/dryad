#include "gtest/gtest.h"
#include "majormode.h"

namespace dryad
{

class mode_graph_should : public ::testing::Test
{
protected:

    mode_graph_should()
    {
    }

    ~mode_graph_should() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};


TEST_F(mode_graph_should, print_some_permutations)
{
    major_mode().print_permutations();

    // Arrange
    // Act
    // Assert
    EXPECT_TRUE(true);
}

}