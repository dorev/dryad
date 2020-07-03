#include "gtest/gtest.h"
#include "degreenode.h"
#include "barlayout.h"

namespace dryad
{

class bar_layout_should : public ::testing::Test
{
protected:

    bar_layout_should()
    {
    }

    ~bar_layout_should() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(bar_layout_should, not_explode_at_construction)
{
    bar_layout b;

    // Arrange
    // Act
    // Assert
}

TEST_F(bar_layout_should, refuse_degree_count_not_matching_pattern)
{
    // Arrange
    bar_layout b;
    degree_node _d(1, triad::major);
    degree_node* d = &_d;

    // Act
    // Assert
    EXPECT_FALSE(b.arrange({ d }, structural_pattern::aabb));
    EXPECT_FALSE(b.arrange({ d }, structural_pattern::aabc));
    EXPECT_FALSE(b.arrange({ d }, structural_pattern::abcd));

    EXPECT_FALSE(b.arrange({ d, d }));
    EXPECT_FALSE(b.arrange({ d, d }, structural_pattern::aabc));
    EXPECT_FALSE(b.arrange({ d, d }, structural_pattern::abcd));

    EXPECT_FALSE(b.arrange({ d, d, d }));
    EXPECT_FALSE(b.arrange({ d, d, d }, structural_pattern::aabb));
    EXPECT_FALSE(b.arrange({ d, d, d }, structural_pattern::abcd));

    EXPECT_FALSE(b.arrange({ d, d, d, d }));
    EXPECT_FALSE(b.arrange({ d, d, d, d }, structural_pattern::aabb));
    EXPECT_FALSE(b.arrange({ d, d, d, d }, structural_pattern::aabc));
}

TEST_F(bar_layout_should, accept_degree_count_not_matching_pattern)
{
    // Arrange
    bar_layout b;
    degree_node _d(1, triad::major);
    degree_node* d = &_d;

    // Act
    // Assert
    EXPECT_TRUE(b.arrange({ d }));
    EXPECT_TRUE(b.arrange({ d, d }, structural_pattern::aabb));
    EXPECT_TRUE(b.arrange({ d, d, d }, structural_pattern::aabc));
    EXPECT_TRUE(b.arrange({ d, d, d, d }, structural_pattern::abcd));
}

}