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
    progression_t prog = m.random_prog(phrase_size, phrase_size);
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
    progression_t prog = m.random_prog(phrase_size, phrase_size);
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

TEST_F(phrase_should, even_compact_right_with_bigger_prog)
{
    // Arrange
    major_mode m;
    m.generate_permutations(0);
    progression_t prog = m.random_prog(6, 6);
    phrase_t ph(4);

    // Act
    ph.set_progression(prog);
    ph.fit_progression(fitting_strategy::even_compact_right);

    // Assert
    EXPECT_EQ(ph[0].get_chords_count(), 1);
    EXPECT_EQ(ph[1].get_chords_count(), 2);
    EXPECT_EQ(ph[2].get_chords_count(), 1);
    EXPECT_EQ(ph[3].get_chords_count(), 2);
}

TEST_F(phrase_should, even_compact_left_with_bigger_prog)
{
    // Arrange
    major_mode m;
    m.generate_permutations(0);
    progression_t prog = m.random_prog(6, 6);
    phrase_t ph(4);

    // Act
    ph.set_progression(prog);
    ph.fit_progression(fitting_strategy::even_compact_left);

    // Assert
    EXPECT_EQ(ph[0].get_chords_count(), 2);
    EXPECT_EQ(ph[1].get_chords_count(), 1);
    EXPECT_EQ(ph[2].get_chords_count(), 2);
    EXPECT_EQ(ph[3].get_chords_count(), 1);
}

TEST_F(phrase_should, even_compact_right_with_smaller_prog)
{
    // Arrange
    major_mode m;
    m.generate_permutations(0);
    progression_t prog = m.random_prog(2, 2);
    phrase_t ph(4);

    // Act
    ph.set_progression(prog);
    ph.fit_progression(fitting_strategy::even_compact_right);

    // !! Should behave like even_compact_left because we can't
    // !! leave a measure empty at the beginning of the phrase
    // Assert
    EXPECT_EQ(ph[0].get_chords_count(), 1);
    EXPECT_EQ(ph[1].get_chords_count(), 0);
    EXPECT_EQ(ph[2].get_chords_count(), 1);
    EXPECT_EQ(ph[3].get_chords_count(), 0);
}

TEST_F(phrase_should, even_compact_left_with_smaller_prog)
{
    // Arrange
    major_mode m;
    m.generate_permutations(0);
    progression_t prog = m.random_prog(2, 2);
    phrase_t ph(4);

    // Act
    ph.set_progression(prog);
    ph.fit_progression(fitting_strategy::even_compact_left);

    // Assert
    EXPECT_EQ(ph[0].get_chords_count(), 1);
    EXPECT_EQ(ph[1].get_chords_count(), 0);
    EXPECT_EQ(ph[2].get_chords_count(), 1);
    EXPECT_EQ(ph[3].get_chords_count(), 0);
}

TEST_F(phrase_should, compact_right_with_bigger_prog)
{
    // Arrange
    major_mode m;
    m.generate_permutations(0);
    progression_t prog = m.random_prog(6, 6);
    phrase_t ph(4);

    // Act
    ph.set_progression(prog);
    ph.fit_progression(fitting_strategy::compact_right);

    // Assert
    EXPECT_EQ(ph[0].get_chords_count(), 1);
    EXPECT_EQ(ph[1].get_chords_count(), 1);
    EXPECT_EQ(ph[2].get_chords_count(), 2);
    EXPECT_EQ(ph[3].get_chords_count(), 2);
}

TEST_F(phrase_should, compact_left_with_bigger_prog)
{
    // Arrange
    major_mode m;
    m.generate_permutations(0);
    progression_t prog = m.random_prog(6, 6);
    phrase_t ph(4);

    // Act
    ph.set_progression(prog);
    ph.fit_progression(fitting_strategy::compact_left);

    // Assert
    EXPECT_EQ(ph[0].get_chords_count(), 2);
    EXPECT_EQ(ph[1].get_chords_count(), 2);
    EXPECT_EQ(ph[2].get_chords_count(), 1);
    EXPECT_EQ(ph[3].get_chords_count(), 1);
}

TEST_F(phrase_should, compact_right_with_smaller_prog)
{
    // Arrange
    major_mode m;
    m.generate_permutations(0);
    progression_t prog = m.random_prog(2, 2);
    phrase_t ph(4);

    // Act
    ph.set_progression(prog);
    ph.fit_progression(fitting_strategy::compact_right);

    // !! Should behave like compact_left because we can't
    // !! leave a measure empty at the beginning of the phrase
    // Assert
    EXPECT_EQ(ph[0].get_chords_count(), 1);
    EXPECT_EQ(ph[1].get_chords_count(), 1);
    EXPECT_EQ(ph[2].get_chords_count(), 0);
    EXPECT_EQ(ph[3].get_chords_count(), 0);
}

TEST_F(phrase_should, compact_left_with_smaller_prog)
{
    // Arrange
    major_mode m;
    m.generate_permutations(0);
    progression_t prog = m.random_prog(2, 2);
    phrase_t ph(4);

    // Act
    ph.set_progression(prog);
    ph.fit_progression(fitting_strategy::compact_left);

    // Assert
    EXPECT_EQ(ph[0].get_chords_count(), 1);
    EXPECT_EQ(ph[1].get_chords_count(), 1);
    EXPECT_EQ(ph[2].get_chords_count(), 0);
    EXPECT_EQ(ph[3].get_chords_count(), 0);
}

}