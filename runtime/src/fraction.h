#pragma once

#include "types.h"
#include <limits>

namespace Dryad 
{
    struct Fraction
    {
        UInt32 numerator;
        UInt32 denominator;

        Fraction(UInt32 numerator = 0, UInt32 denominator = 0)
            : numerator(numerator)
            , denominator(denominator)
        {
        }

        static UInt32 GreatestCommonDivisor(UInt32 leftFraction, UInt32 rightFraction)
        {
            while (rightFraction != 0)
            {
                UInt32 temp = rightFraction;
                rightFraction = leftFraction % rightFraction;
                leftFraction = temp;
            }
            return leftFraction;
        }

        Fraction& Simplify()
        {
            UInt32 commonDivisor = GreatestCommonDivisor(numerator, denominator);
            numerator /= commonDivisor;
            denominator /= commonDivisor;
            return *this;
        }

        Fraction operator+(const Fraction& other) const
        {
            UInt32 commonDenominator = denominator * other.denominator;
            UInt32 newNumerator = numerator * other.denominator + other.numerator * denominator;
            return Fraction(newNumerator, commonDenominator);
        }

        Fraction& operator+=(const Fraction& other)
        {
            UInt32 commonDenominator = denominator * other.denominator;
            UInt32 newNumerator = numerator * other.denominator + other.numerator * denominator;
            numerator = newNumerator;
            denominator = commonDenominator;
            return *this;
        }

        Fraction operator-(const Fraction& other) const
        {
            UInt32 commonDenominator = denominator * other.denominator;
            if (numerator * other.denominator < other.numerator * denominator)
            {
                // Result is negative, return 0
                return Fraction(0, commonDenominator);
            }
            UInt32 newNumerator = numerator * other.denominator - other.numerator * denominator;
            Fraction result(newNumerator, commonDenominator);
            return result;
        }

        Fraction& operator-=(const Fraction& other)
        {
            UInt32 commonDenominator = denominator * other.denominator;
            if (numerator * other.denominator < other.numerator * denominator)
            {
                // Result is negative, set to 0
                numerator = 0;
                denominator = commonDenominator;
            }
            else
            {
                UInt32 newNumerator = numerator * other.denominator - other.numerator * denominator;
                numerator = newNumerator;
                denominator = commonDenominator;
            }
            return *this;
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

    };
}