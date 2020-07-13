#include "gtest/gtest.h"
#include "composer.h"
#include "majormode.h"

namespace dryad
{

class phrase_should : public ::testing::Test
{
protected:

    phrase_should()
    {
    }

    ~phrase_should() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(phrase_should, fit_perfectly_progression_of_same_size_4)
{
    // Arrange
    size_t phrase_size = 4;
    major_mode m;
    m.generate_permutations(0);
    progression prog = m.random_prog(phrase_size, phrase_size);
    phrase ph(phrase_size);

    // Act
    ph.apply_progression(prog);

    // Assert
    for (size_t i = 0; i < ph.size(); ++i)
    {
        EXPECT_EQ(ph[i].size(), 1);
    }
}

TEST_F(phrase_should, fit_perfectly_progression_of_same_size_8)
{
    // Arrange
    size_t phrase_size = 8;
    major_mode m;
    m.generate_permutations(0);
    progression prog = m.random_prog(phrase_size, phrase_size);
    phrase ph(phrase_size);

    // Act
    ph.apply_progression(prog);

    // Assert
    for (size_t i = 0; i < ph.size(); ++i)
    {
        EXPECT_EQ(ph[i].size(), 1);
    }
}

TEST_F(phrase_should, compact_even_right_with_bigger_prog_4)
{
    // Arrange
    major_mode m;
    m.generate_permutations(0);
    progression prog = m.random_prog(6, 6);
    phrase ph(4);

    // Act
    ph.apply_progression(prog);

    // Assert
    EXPECT_EQ(ph[0].size(), 1);
    EXPECT_EQ(ph[1].size(), 2);
    EXPECT_EQ(ph[2].size(), 1);
    EXPECT_EQ(ph[3].size(), 2);
}

TEST_F(phrase_should, compact_even_right_with_bigger_prog_8)
{
    // Arrange
    major_mode m;
    m.generate_permutations(0);
    progression prog = m.random_prog(10, 10);
    phrase ph(8);

    // Act
    ph.apply_progression(prog);

    // Assert
    EXPECT_EQ(ph[0].size(), 1);
    EXPECT_EQ(ph[1].size(), 1);
    EXPECT_EQ(ph[2].size(), 1);
    EXPECT_EQ(ph[3].size(), 2);
    EXPECT_EQ(ph[4].size(), 1);
    EXPECT_EQ(ph[5].size(), 1);
    EXPECT_EQ(ph[6].size(), 1);
    EXPECT_EQ(ph[7].size(), 2);
}

}