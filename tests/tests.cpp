#include "gtest/gtest.h"
#include "dryad/src/system.h"

using namespace Dryad;

class Tests : public ::testing::Test
{
};

TEST_F(Tests, PrintFrequency)
{
    printf("C4 frequency is %f\n", Frequencies[C][4]);
}

