#include "gtest/gtest.h"
#include "dryad.h"

namespace dryad
{
namespace harmony
{

using namespace model;

class harmony_tests : public ::testing::Test
{
protected:

    harmony_tests()
    {
    }

    ~harmony_tests() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(harmony_tests, create_a_valid_major_scale)
{
    // Arrange
    // Act
    // Assert

    scale_ptr scale = create_major_scale();
    EXPECT_EQ(scale->degrees.size(), 7);

}

} // namespace harmony
} // namespace dryad
