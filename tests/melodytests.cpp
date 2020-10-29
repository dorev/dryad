#include "gtest/gtest.h"
#include "melody.h"
#include <execution>

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
    int n = 100;
    int epochs = 10;
    std::vector<int> durations = { HALF, HALF + QUARTER, WHOLE, WHOLE + HALF, 2 * WHOLE, 3 * WHOLE };
    std::vector<int> notes_count = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

    dryad_timer timer;
    std::vector<double> times;
    times.reserve(static_cast<size_t>(n));
    double max_time = 0;

    for_range(j, epochs)
    {
        max_time = 0;
        times.clear();

        int requested_duration = random::in(durations);
        int requested_notes_count = random::in(notes_count);

        std::cout << "Generating " << n << " melodies of " << requested_notes_count
                  << " notes with requested duration of "<< static_cast<double>(requested_duration)/ static_cast<double>(QUARTER)
                  << " beats\n";

        for_range(i, n)
        {
            // Act
            timer.reset();
            melody m(requested_duration, requested_notes_count);
            double time = timer.stop_ms();
            times.push_back(time);
            if (time > max_time) { max_time = time; }

            // Assert
            if (m.get_total_duration() != requested_duration)
            {
                FAIL() << "Unable to fit notes durations to requested melody duration";
            }
        }

        double avg = std::reduce(std::execution::par_unseq, times.begin(), times.end()) / static_cast<double>(n);
        std::cout << "Average melody generation time : " << std::to_string(avg) << " ms (max : " << std::to_string(max_time) << " ms)\n";
    }
}


TEST_F(melody_should, successfully_extend)
{
    std::vector<int> initial_durations = { HALF, HALF + EIGHTH, HALF_DOTTED, WHOLE - EIGHTH, WHOLE };
    std::vector<int> target_durations = { WHOLE, WHOLE + EIGHTH, WHOLE + QUARTER, WHOLE + HALF, 2 * WHOLE };

    for (unsigned int i = 0; i < initial_durations.size(); ++i)
    {
        for (unsigned int j = 0; j < target_durations.size(); ++j)
        {
            int epochs = 10;
            while (epochs--)
            {
                // Arrange
                int initial_duration = initial_durations[i];
                int target_duration = target_durations[j];
                melody m(initial_duration, random::range(4, 8));

                // Act
                m.resize(target_duration);

                // Assert
                int extended_duration = m.get_total_duration();

                if (extended_duration != target_duration)
                {
                    FAIL() << "Failed to extend melody from duration "
                        << initial_duration << " to " << target_duration;
                }
            }
        }
    }
}

TEST_F(melody_should, successfully_shrink)
{

    std::vector<int> initial_durations = { WHOLE, WHOLE + EIGHTH, WHOLE + QUARTER, WHOLE + HALF, 2 * WHOLE };
    std::vector<int> target_durations = { HALF, HALF + EIGHTH, HALF_DOTTED, WHOLE - EIGHTH, WHOLE };

    for (unsigned int i = 0; i < initial_durations.size(); ++i)
    {
        for (unsigned int j = 0; j < target_durations.size(); ++j)
        {
            int epochs = 10;
            while (epochs--)
            {
                // Arrange
                int initial_duration = initial_durations[i];
                int target_duration = target_durations[j];

                melody m(initial_duration, random::range(4, 8));

                // Act
                m.resize(target_duration);

                // Assert
                int shrinked_duration = m.get_total_duration();

                if (shrinked_duration != target_duration)
                {
                    FAIL() << "Failed to shrink melody from duration "
                        << initial_duration << " to " << target_duration;
                }
            }
        }
    }
}

}