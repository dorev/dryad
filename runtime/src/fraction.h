#pragma once

#include "types.h"
#include <limits>

namespace Dryad 
{
    struct Fraction
    {
        Fraction(UInt32 numerator = 0, UInt32 denominator = 0)
            : numerator(numerator)
            , denominator(denominator)
        {
        }

        bool IsZero() const
        {
            return numerator == 0;
        }

        bool IsValid() const
        {
            return denominator > 0;
        }

        float AsFloat() const
        {
            return IsValid()
                ? static_cast<float>(numerator) / static_cast<float>(denominator)
                : std::numeric_limits<float>::max();
        }

        bool operator>(float value) const
        {
            return AsFloat() > value;
        }

        bool operator<(float value) const
        {
            return AsFloat() < value;
        }

        // TODO: implement a rational comparison
        bool operator>(const Fraction& other) const
        {
            return AsFloat() > other.AsFloat();
        }

        // TODO: implement a rational comparison
        bool operator<(const Fraction& other) const
        {
            return AsFloat() > other.AsFloat();
        }

        bool operator==(float value) const
        {
            return AsFloat() == value;
        }

        UInt32 numerator;
        UInt32 denominator;
    };
}