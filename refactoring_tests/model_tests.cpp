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

    int epoch = 1000;

    for (int n = 0; n < epoch; ++n)
    {
        EXPECT_EQ(0ULL, phrase_t::get_count());
        EXPECT_EQ(0ULL, measure_t::get_count());

        phrase_ptr phrase = make_phrase();

        for (int i = 0; i < 8; ++i)
        {
            phrase->measures.emplace_back(make_measure());
        }
        const std::vector<harmony_node_ptr>& progression = random::in(graph->progressions);

        apply_progression(phrase, progression);

        int expected_harmony_nodes_count = static_cast<int>(progression.size());
        int actual_harmony_nodes_count = 0;

        for (measure_ptr& measure : phrase->measures)
        {
            actual_harmony_nodes_count += static_cast<int>(measure->progression.size());
        }

        if (expected_harmony_nodes_count != actual_harmony_nodes_count)
        {
            FAIL() << "failed during epoch " << n << ": expected " << expected_harmony_nodes_count << " harmony_nodes in measures but found " << actual_harmony_nodes_count;
        }

        EXPECT_EQ(1ULL, phrase_t::get_count());
        EXPECT_EQ(8ULL, measure_t::get_count());
    }
}


TEST_F(model_tests, apply_motif_to_phrase)
{
    int epoch = 1000;
    voice_ptr voice = make_voice();


    for (int n = 0; n < epoch; ++n)
    {
        phrase_ptr phrase = make_phrase();

        int measure_count = random::in(std::vector<int>{4, 8, 16});

        for (int i = 0; i < measure_count; ++i)
        {
            append_measure(phrase, make_measure());
        }

        motif_variation_ptr motif = make_motif_variation();
        motif_config_ptr motif_config = make_motif_config();

        int duration = motif_config->duration = random::in(std::vector<int>{_half_, _half_dotted_, _whole_, _whole_dotted_});
        motif_config->rhythmic_energy = random::range(duration / _half_, duration / _sixteenth_);
        motif_config->melodic_energy = random::range(0, 100);
        int min = motif_config->min_melodic_energy = random::range(0, 4);
        motif_config->max_melodic_energy = random::range(min, 16);

        generate_motif(motif, motif_config);

        apply_motif(phrase, motif, voice);

        // evaluate with position count and presence of voice and note duration values
        int cumulative_duration_of_notes = 0;

        for (measure_ptr measure : phrase->measures)
        {
            for (position_ptr position : measure->positions)
            {
                EXPECT_EQ(1ULL, position->notes.size()) << "failed during epoch " << n << ": each position should only contain one note, this one contains " << position->notes.size();

                note_ptr& note = position->notes[0];

                if (note->voice == nullptr)
                {
                    FAIL() << "failed during epoch " << n << ": Note should be assigned to a voice";
                }

                cumulative_duration_of_notes += note->duration;
            }
        }

        int expected_duration = measure_count * _whole_;
        if (cumulative_duration_of_notes != expected_duration)
        {
            FAIL() << "failed during epoch " << n << ": expected total duration of " << expected_duration << " but actual value is " << cumulative_duration_of_notes;
        }
    }
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
