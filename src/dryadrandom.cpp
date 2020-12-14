#include "dryadrandom.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//int random::range(int min, int max)
//{
//    if (min == max)
//    {
//        return min;
//    }
//
//    static thread_local std::mt19937 generator(std::random_device{}());
//    std::uniform_int_distribution<int> distribution(min, max);
//    return distribution(generator);
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int random::normal_int(float mean, float stddev)
{
    static thread_local std::mt19937 generator(std::random_device{}());
    std::normal_distribution<float> distribution(mean, stddev);
    return static_cast<int>(distribution(generator));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool random::coin_flip()
{
    static thread_local std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(0, 1);
    return distribution(generator) > 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}