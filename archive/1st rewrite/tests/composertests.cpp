#include "gtest/gtest.h"
#include "composer.h"
#include "majormode.h"
#include <fstream>

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
        .add_phrase(0, phrase_t(4))
        .add_phrase(1)
        .add_phrase(2)
        .set_phrase_sequence({ 0, 1, 1, 0, 2 });
}

TEST_F(composer_should, be_executable)
{
    int epochs = 20;

    std::cout << "Running " << epochs << " epochs of test\n";

    while (epochs--)
    {
        // Arrange
        composer c;
        major_mode m;

        // Act
        // Assert
        c.set_mode(&m)
            .add_phrase(0)
            .add_phrase(1)
            .add_phrase(2)
            .set_phrase_sequence({ 0, 1, 1, 0, 2 })
            .execute();
    }
}


TEST_F(composer_should, print_musicxml)
{
    // Arrange
    composer c;
    major_mode m;

    // Act
    // Assert
    std::ofstream file(R"(C:\Users\Vincent\Desktop\Untitled-1.xml)");

    file << c.set_mode(&m)
        .add_phrase(0)
        .add_phrase(1)
        .add_phrase(2)
        .add_melody(0, {0, 1})
        .add_melody(1, {1, 2})
        .set_phrase_sequence({ 0, 1, 0, 2 })
            .execute();

    file.close();
}


}