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
    int iterations = 1000;
    std::vector<int> durations = { HALF, HALF + QUARTER, WHOLE, WHOLE + HALF, 2 * WHOLE, 3 * WHOLE };
    std::vector<int> notes_count = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

    dryad_timer timer;
    std::vector<double> times;
    times.reserve(static_cast<size_t>(iterations));
    double max_time = 0;

    for_range(j, 20)
    {
        max_time = 0;
        times.clear();

        int requested_duration = random::in(durations);
        int requested_notes_count = random::in(notes_count);

        std::cout << "Generating " << iterations << " melodies of " << requested_notes_count
                  << " notes with requested duration of "<< static_cast<double>(requested_duration)/ static_cast<double>(QUARTER)
                  << " beats\n";

        for_range(i, iterations)
        {
            // Act
            timer.reset();
            melody m(requested_duration, requested_notes_count);
            double time = timer.stop_ms();
            times.push_back(time);
            if (time > max_time) { max_time = time; }

            // Assert
            auto durations = m.get_durations().value();
            int total_duration = std::reduce(durations.begin(), durations.end(), 0);

            if (total_duration != requested_duration)
            {
                FAIL() << "Unable to fit notes durations to requested melody duration";
            }
        }

        double avg = std::reduce(std::execution::par_unseq, times.begin(), times.end()) / static_cast<double>(iterations);
        std::cout << "Average melody generation time : " << std::to_string(avg) << " ms (max : " << std::to_string(max_time) << " ms)\n";
    }
}

}