#include "gtest/gtest.h"
#include "dryad/core/core.h"

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

#define EXPECT_NO_LEAK_FROM_PREVIOUS_TEST               \
    EXPECT_EQ(0ULL, degree_t::get_count());             \
    EXPECT_EQ(0ULL, harmony_graph_t::get_count());      \
    EXPECT_EQ(0ULL, harmony_node_t::get_count());       \
    EXPECT_EQ(0ULL, measure_t::get_count());            \
    EXPECT_EQ(0ULL, motif_t::get_count());              \
    EXPECT_EQ(0ULL, motif_variation_t::get_count());    \
    EXPECT_EQ(0ULL, position_t::get_count());           \
    EXPECT_EQ(0ULL, note_t::get_count());               \
    EXPECT_EQ(0ULL, phrase_t::get_count());             \
    EXPECT_EQ(0ULL, scale_t::get_count());              \
    EXPECT_EQ(0ULL, voice_t::get_count());              \

TEST_F(model_tests, apply_progression_to_phrase)
{
    EXPECT_NO_LEAK_FROM_PREVIOUS_TEST

    harmony_graph_ptr graph = create_major_graph();
    generate_progressions(graph);

    int epoch = 100;

    for (int n = 0; n < epoch; ++n)
    {
        EXPECT_EQ(0ULL, phrase_t::get_count());
        EXPECT_EQ(0ULL, measure_t::get_count());

        phrase_ptr phrase = make_phrase();

        for (int i = 0; i < 8; ++i)
        {
            phrase->measures.emplace_back(make_measure());
        }
        const std::vector<harmony_node_weak_ptr>& progression = random::in(graph->progressions);

        apply_progression(phrase, progression);

        int expected_harmony_nodes_count = static_cast<int>(progression.size());
        int actual_harmony_nodes_count = 0;

        for (measure_ptr& measure : phrase->measures)
        {
            actual_harmony_nodes_count += static_cast<int>(measure->progression.size());
        }

        if (expected_harmony_nodes_count != actual_harmony_nodes_count)
        {
            FAIL() << "failed during epoch " << n << ": expected " << expected_harmony_nodes_count
                << " harmony_nodes in measures but found " << actual_harmony_nodes_count;
        }

        EXPECT_EQ(1ULL, phrase_t::get_count());
        EXPECT_EQ(8ULL, measure_t::get_count());
    }
}

TEST_F(model_tests, apply_motif_to_phrase)
{
    EXPECT_NO_LEAK_FROM_PREVIOUS_TEST

    int epoch = 100;
    voice_ptr voice = make_voice();

    for (int n = 0; n < epoch; ++n)
    {
        EXPECT_EQ(note_t::get_count(), 0ULL);

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
                EXPECT_EQ(1ULL, position->notes.size()) << "failed during epoch " << n 
                    << ": each position should only contain one note, this one contains " << position->notes.size();

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
            FAIL() << "failed during epoch " << n << ": expected total duration of " << expected_duration
                << " but actual value is " << cumulative_duration_of_notes;
        }
    }
}

TEST_F(model_tests, apply_scale_to_score)
{
    EXPECT_NO_LEAK_FROM_PREVIOUS_TEST

    int epoch = 100;

    score_ptr score = create_score();
    score->graph = create_major_graph();
    generate_progressions(score->graph);
    scale_ptr scale = create_major_scale();

    for (int n = 0; n < epoch; ++n)
    {
        EXPECT_EQ(phrase_t::get_count(), 0ULL);
        EXPECT_EQ(measure_t::get_count(), 0ULL);
        EXPECT_EQ(position_t::get_count(), 0ULL);

        //EXPECT_EQ(note_t::get_count(), 0ULL);

        std::vector<int> counts = {2, 4, 8, 16};
        int phrase_count = random::in(counts);
        int measures_by_phrase = random::in(counts);

        for (int i = 0; i < phrase_count; ++i)
        {
            append_phrase(score, make_phrase());
        }

        motif_variation_ptr motif = make_motif_variation();

        motif_config_ptr motif_config = make_motif_config();
        int duration = motif_config->duration = random::in(std::vector<int>{_half_, _half_dotted_, _whole_, _whole_dotted_});
        motif_config->rhythmic_energy = random::range(duration / _half_, duration / _sixteenth_);
        motif_config->melodic_energy = random::range(0, 100);
        int min = motif_config->min_melodic_energy = random::range(0, 4);
        motif_config->max_melodic_energy = random::range(min, 16);

        generate_motif(motif, motif_config);

        voice_ptr voice = make_voice();

        for (phrase_ptr phrase : score->phrases)
        {
            for (int i = 0; i < measures_by_phrase; ++i)
            {
                append_measure(phrase, make_measure());
            }
        }

        for (phrase_ptr phrase : score->phrases)
        {
            apply_progression(phrase, random::in(score->graph->progressions));
            apply_motif(phrase, motif, voice);
        }

        scale_config_ptr scale_config = make_scale_config(random::range(0,11), (accidental_e)random::range(0,2));

        apply_scale(score, scale, scale_config);

        position_ptr score_position = score->phrases[0]->measures[0]->positions[0];

        while (score_position != nullptr)
        {
            if (score_position->notes.size() == 0)
            {
                FAIL() << "failed during epoch " << n << ": a position was created without notes";
            }

            for (note_ptr note : score_position->notes)
            {
                if (note->midi < 0)
                {
                    FAIL() << "failed during epoch " << n << ": a note was created without midi value";
                }
                else if (note->duration <= 0)
                {
                    FAIL() << "failed during epoch " << n << ": a note was created with an invalid duration";
                }
                else if (note->octave < 0)
                {
                    FAIL() << "failed during epoch " << n << ": a note was created without an octave value";
                }
            }

            score_position = next(score_position);
        }

        score->phrases.clear();
    }
}

TEST_F(model_tests, create_a_valid_major_scale)
{
    EXPECT_NO_LEAK_FROM_PREVIOUS_TEST

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

TEST_F(model_tests, generate_major_graph_progressions)
{
    EXPECT_NO_LEAK_FROM_PREVIOUS_TEST

    harmony_graph_ptr graph = create_major_graph();
    generate_progressions(graph);

    EXPECT_EQ(graph->progressions.size(), 1384) << "1384 permutations are expected from default major mode";
}

TEST_F(model_tests, spend_melodic_energy_correctly)
{
    EXPECT_NO_LEAK_FROM_PREVIOUS_TEST

    motif_ptr motif = make_motif();
    motif_config_ptr motif_config = make_motif_config();

    int epoch = 100;
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

TEST_F(model_tests, spend_rhythmic_energy_correctly)
{
    EXPECT_NO_LEAK_FROM_PREVIOUS_TEST

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

TEST_F(model_tests, empty_test_DISABLED)
{
    EXPECT_NO_LEAK_FROM_PREVIOUS_TEST

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
