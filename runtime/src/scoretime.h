#pragma once

#include "types.h"
#include "fraction.h"

namespace Dryad 
{
    using Time = float;
    using Tempo = float;
    const Tempo DefaultTempo = 108;

    struct ScoreTime : Fraction
    {
        ScoreTime(UInt32 numerator = 0, UInt32 denominator = 0, Time time = 0)
            : Fraction(numerator, denominator)
            , time(time)
        {
        }

        ScoreTime operator+(const ScoreTime& other) const
        {
            ScoreTime result = *this;
            result += other;
            return result;
        }

        ScoreTime& operator+=(const ScoreTime& other)
        {
            Fraction::operator+=(other);
            time += other.time;
            return *this;
        }

        ScoreTime operator-(const ScoreTime& other) const
        {
            ScoreTime result = *this;
            result -= other;
            return result;
        }

        ScoreTime& operator-=(const ScoreTime& other)
        {
            Fraction::operator-=(other);
            time -= other.time;
            if (time < 0)
            {
                time = 0;
            }
            return *this;
        }

        bool LesserThan(UInt32 numerator, UInt32 denominator) const
        {
            return (this->numerator * denominator) < (numerator * this->denominator);
        }

        bool GreaterThan(UInt32 numerator, UInt32 denominator) const
        {
            return (this->numerator * denominator) > (numerator * this->denominator);
        }

        Time time;
    };

    using TimeSignature = Fraction;
}