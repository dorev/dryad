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

TEST_F(mode_graph_should, not_explode_at_construction)
{
    major_mode m;
    m.generate_permutations();

    // Arrange
    // Act
    // Assert
}

TEST_F(mode_graph_should, not_explode_with_multiple_instances)
{
    major_mode m1;
    m1.generate_permutations();

    major_mode m2;
    m2.generate_permutations();

    major_mode m3;
    m3.generate_permutations();

    major_mode m4;
    m4.generate_permutations();

    // Arrange
    // Act
    // Assert
}

TEST_F(mode_graph_should, print_max_permutations)
{
    major_mode m;
    m.generate_permutations(0);
    m.print_permutations();

    // Arrange
    // Act
    // Assert
}

}