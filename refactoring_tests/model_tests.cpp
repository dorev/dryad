#include "gtest/gtest.h"
#include "dryad.h"

namespace dryad
{

class model_tests : public ::testing::Test
{
protected:

    model_tests()
    {
    }

    ~model_tests() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(model_tests, apply_progression_to_phrase)
{
    harmony_graph_ptr graph = create_major_graph();
    generate_progressions(graph);

    int epoch = 100;

    for (int n = 0; n < epoch; ++n)
    {
        phrase_ptr phrase = make_phrase();

        for (int i = 0; i < 8; ++i)
        {
            phrase->measures.emplace_back(make_measure());
        }
        const std::vector<harmony_node_ptr>& progression = random::in(graph->progressions);

        apply_progression(phrase, progression);

        if (false)
        {
            FAIL() << "failed during epoch " << n << ": ";
        }
    }
}


TEST_F(model_tests, apply_motif_to_phrase)
{
    
}

TEST_F(model_tests, empty_test)
{
    int epoch = 100;

    for (int n = 0; n < epoch; ++n)
    {
        if (false)
        {
            FAIL() << "failed during epoch " << n << ": ";
        }
    }
}

} // namespace dryad
