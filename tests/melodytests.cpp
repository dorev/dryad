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
    int requested_duration = WHOLE;
    int iterations = 1000;

    dryad_timer timer;
    std::vector<double> times;
    times.reserve(static_cast<size_t>(iterations));
    double max_time = 0;

    for_range(j, 20)
    {
        max_time = 0;
        times.clear();

        for_range(i, iterations)
        {
            // Act
            timer.reset();
            melody m(requested_duration, 4);
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

        double avg = std::reduce(std::execution::par_unseq, times.begin(), times.end()) / 1000.0;
        std::cout << "Average melody generation time : " << std::to_string(avg) << " ms (max : " << std::to_string(max_time) << " ms)\n";
    }
}

}