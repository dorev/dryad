#pragma once

#include <random>
#include "result.h"
#include "containers.h"

namespace Dryad
{
    template <class T>
    Result RandomFrom(const Vector<T>& vector, const T& output)
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

    template <class T>
    Result RandomFrom(const Vector<T*>& vector, const T*& output)
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

    template <class T, class U>
    auto Random(T min, U max) -> decltype(min + max)
    {
        static thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<decltype(min + max)> distribution(min, max);
        return distribution(generator);
    }
}