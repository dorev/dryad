#pragma once

#include <random>
#include "result.h"
#include "containers.h"

namespace Dryad
{
    template <typename T>
    Result RandomFrom(const Vector<T>& vector, T& output)
    {
        if (vector.Empty())
        {
            return Result::ContainerEmpty;
        }
        static thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<unsigned int> distribution(0, vector.Size() - 1);
        int index = distribution(generator);
        output = vector[index];
        return Result::Success;
    }
}