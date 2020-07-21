#include "gtest/gtest.h"
#include "melody.h"

namespace dryad
{

class melody_should : public ::testing::Test
{
protected:

    melody_should()
    {
    }

    ~melody_should() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(melody_should, have_the_specified_duration)
{
    // Arrange
    // Act
    // Assert
    int requested_duration = WHOLE;
    melody m(requested_duration, 4);

    auto durations = m.get_durations().value();

    int total_duration = std::reduce(durations.begin(), durations.end(), 0);
    EXPECT_EQ(total_duration, requested_duration);
}

}