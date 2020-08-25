#include "gtest/gtest.h"
#include "composer.h"
#include "majormode.h"

namespace dryad
{

class composer_should : public ::testing::Test
{
protected:

    composer_should()
    {
    }

    ~composer_should() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(composer_should, not_explode_at_construction)
{
    composer c;

    // Arrange
    // Act
    // Assert
}

TEST_F(composer_should, be_buildable)
{
    // Arrange
    composer c;
    major_mode m;

    // Act
    // Assert
    c.set_mode(&m)
        .add_phrase(0, phrase(4))
        .add_phrase(1, phrase())
        .add_phrase(2, phrase())
        .set_phrase_sequence({ 0, 1, 1, 0, 2 });
}

TEST_F(composer_should, be_executable)
{
    // Arrange
    composer c;
    major_mode m;

    // Act
    // Assert
    c.set_mode(&m)
        .add_phrase(0, phrase())
        .add_phrase(1, phrase())
        .add_phrase(2, phrase())
        .set_phrase_sequence({ 0, 1, 1, 0, 2 })
        .execute();
}


}