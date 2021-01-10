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
    EXPECT_EQ(0ULL, degree_t::get_count());

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
    degree_ptr ii_again = previous(previous(previous(previous(previous(previous(previous(ii)))))));

    EXPECT_EQ(ii, ii_again) << "Degrees are not linked correctly";
    EXPECT_EQ(7ULL, degree_t::get_count());
}

TEST_F(harmony_tests, generate_major_graph_progressions)
{
    harmony_graph_ptr graph = create_major_graph();
    generate_progressions(graph);

    EXPECT_EQ(graph->progressions.size(), 1384) << "1384 permutations are expected from default major mode";
}

TEST_F(harmony_tests, spend_melodic_energy_correctly)
{
    motif_ptr motif = make_motif();
    motif_config_ptr motif_config = make_motif_config();

    int epoch = 1000;
    int note_count = 8;

    for (int n = 0; n < epoch; ++n)
    {
        motif->variations.emplace_back(make_motif_variation());
        motif_variation_ptr variation = motif->variations[n];

        motif_config->duration = 2 * _whole_;
        int min_energy = motif_config->min_melodic_energy = random::range(1, 3);
        int max_energy = motif_config->max_melodic_energy = random::range(min_energy + 1, 12);

        int total_energy = motif_config->melodic_energy = random::range(note_count * min_energy / 2, (int)(note_count * max_energy * 1.25));

        for (int i = 0; i < note_count; ++i)
        {
            variation->notes.emplace_back(make_note())->duration = _quarter_;
        }

        spend_melodic_energy(variation, motif_config);

        int spent_energy =
            std::reduce
            (
                variation->notes.begin(),
                variation->notes.end(),
                0,
                [](int acc, const note_ptr& note)
                {
                    return acc + std::abs(note->offset);
                }
            );

        int expected_energy_spent = std::min(total_energy, note_count * max_energy);

        if (spent_energy != expected_energy_spent)
        {
            FAIL() << "failed during epoch " << n << ": energy spent is " << spent_energy << " while expected spending was " << expected_energy_spent;
        }
    }
}

TEST_F(harmony_tests, spend_rhythmic_energy_correctly)
{
    motif_ptr motif = make_motif();
    motif_config_ptr motif_config = make_motif_config();

    int epoch = 100;

    for (int n = 0; n < epoch; ++n)
    {
        motif->variations.emplace_back(make_motif_variation());
        motif_variation_ptr variation = motif->variations[n];

        motif_config->duration = 2 * _whole_;
        int max_notes = motif_config->duration / _sixteenth_;

        int rhythmic_energy = motif_config->rhythmic_energy = random::range(0, max_notes);

        spend_rhythmic_energy(variation, motif_config);

        int expected_note_count = std::min(rhythmic_energy + 1, max_notes);
        int actual_note_count = (int)variation->notes.size();

        if (actual_note_count != expected_note_count)
        {
            FAIL() << "failed during epoch " << n << ": should have splitted motif in " << expected_note_count << " notes but made " << actual_note_count;
        }
    }
}

TEST_F(harmony_tests, empty_test)
{

}

} // namespace dryad
