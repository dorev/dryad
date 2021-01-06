#include "gtest/gtest.h"
#include "dryad.h"

namespace dryad
{

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
    scale_ptr scale = create_major_scale();
    EXPECT_EQ(scale->degrees.size(), 7) << "Scale has wrong size";

    EXPECT_EQ(scale->degrees[0]->interval_from_root, 0) << "Wrong inteval for I";
    EXPECT_EQ(scale->degrees[1]->interval_from_root, 2) << "Wrong inteval for ii";
    EXPECT_EQ(scale->degrees[2]->interval_from_root, 4) << "Wrong inteval for iii";
    EXPECT_EQ(scale->degrees[3]->interval_from_root, 5) << "Wrong inteval for IV";
    EXPECT_EQ(scale->degrees[4]->interval_from_root, 7) << "Wrong inteval for V";
    EXPECT_EQ(scale->degrees[5]->interval_from_root, 9) << "Wrong inteval for vi";
    EXPECT_EQ(scale->degrees[6]->interval_from_root, 11) << "Wrong inteval for viid";

    degree_ptr I = scale->degrees[0];
    degree_ptr I_again = next(next(next(next(next(next(next(I)))))));
    EXPECT_EQ(I, I_again) << "Degrees are not linked correctly";

    degree_ptr ii = scale->degrees[1];
    degree_ptr ii_again = prev(prev(prev(prev(prev(prev(prev(ii)))))));
    EXPECT_EQ(ii, ii_again) << "Degrees are not linked correctly";
}

TEST_F(harmony_tests, generate_major_graph_progressions)
{
    harmony_graph_ptr graph = create_major_graph();
    generate_progressions(graph);

    EXPECT_EQ(graph->progressions.size(), 1384) << "1384 permutarions are expected from default major mode";
}
} // namespace dryad
