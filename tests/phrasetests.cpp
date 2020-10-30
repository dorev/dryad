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
    phrase_t ph(phrase_size);

    // Act
    ph.set_progression(prog);
    ph.fit_progression();

    // Assert
    for (size_t i = 0; i < ph.size(); ++i)
    {
        EXPECT_EQ(ph[i].get_chords_count(), 1);
    }
}

TEST_F(phrase_should, fit_perfectly_progression_of_same_size_8)
{
    // Arrange
    size_t phrase_size = 8;
    major_mode m;
    m.generate_permutations(0);
    progression prog = m.random_prog(phrase_size, phrase_size);
    phrase_t ph(phrase_size);

    // Act
    ph.set_progression(prog);
    ph.fit_progression();

    // Assert
    for (size_t i = 0; i < ph.size(); ++i)
    {
        EXPECT_EQ(ph[i].get_chords_count(), 1);
    }
}

TEST_F(phrase_should, compact_even_right_with_bigger_prog_4)
{
    // Arrange
    major_mode m;
    m.generate_permutations(0);
    progression prog = m.random_prog(6, 6);
    phrase_t ph(4);

    // Act
    ph.set_progression(prog);
    ph.fit_progression();

    // Assert
    EXPECT_EQ(ph[0].get_chords_count(), 1);
    EXPECT_EQ(ph[1].get_chords_count(), 2);
    EXPECT_EQ(ph[2].get_chords_count(), 1);
    EXPECT_EQ(ph[3].get_chords_count(), 2);
}

TEST_F(phrase_should, compact_even_right_with_bigger_prog_8)
{
    // Arrange
    major_mode m;
    m.generate_permutations(0);
    progression prog = m.random_prog(10, 10);
    phrase_t ph(8);

    // Act
    ph.set_progression(prog);
    ph.fit_progression();

    // Assert
    EXPECT_EQ(ph[0].get_chords_count(), 1);
    EXPECT_EQ(ph[1].get_chords_count(), 1);
    EXPECT_EQ(ph[2].get_chords_count(), 1);
    EXPECT_EQ(ph[3].get_chords_count(), 2);
    EXPECT_EQ(ph[4].get_chords_count(), 1);
    EXPECT_EQ(ph[5].get_chords_count(), 1);
    EXPECT_EQ(ph[6].get_chords_count(), 1);
    EXPECT_EQ(ph[7].get_chords_count(), 2);
}

}