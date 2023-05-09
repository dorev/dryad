#pragma once

#include "types.h"

namespace Dryad
{

    enum Accidental : UInt8
    {
        Natural,
        Flat,
        Sharp,
    };

    enum QualityFlags : UInt32
    {
        Minor = 0,
        Major = 1,
        Diminished = 2,
        Augmented = 3,
        Sus2 = 4,
        Sus4 = 5,
        DiminishedSeventh = 6,
        AugmentedSeventh = 7,
        Seventh = 1 << 4,
        MajorSeventh = 1 << 5,
        Sixth = 1 << 6,
        Ninth = 1 << 7,
        Eleventh = 1 << 8,
        Thirteenth = 1 << 9,
        FlatSecond = 1 << 10,
        FlatThird = 1 << 11,
        FlatFourth = 1 << 12,
        FlatFifth = 1 << 13,
        FlatSixth = 1 << 14,
        FlatSeventh = 1 << 15,
        FlatNinth = 1 << 16,
        FlatEvelenth = 1 << 17,
        FlatThirteenth = 1 << 18,
        SharpSecond = 1 << 19,
        SharpThird = 1 << 20,
        SharpFourth = 1 << 21,
        SharpFifth = 1 << 22,
        SharpSixth = 1 << 23,
        SharpSeventh = 1 << 24,
        SharpNinth = 1 << 25,
        SharpEvelenth = 1 << 26,
        SharpThirteenth = 1 << 27,
        //PLACEHOLDER_BIT0 = 1 << 28,
        //PLACEHOLDER_BIT1 = 1 << 29,
        //PLACEHOLDER_BIT2 = 1 << 30,
        //PLACEHOLDER_BIT3 = 1 << 31,
    };

    enum DegreeFlags : UInt8
    {
        Tonic = 1 << 1,
        Supertonic = 1 << 2,
        Mediant = 1 << 3,
        Subdominant = 1 << 4,
        Dominant = 1 << 5,
        Submediant = 1 << 6,
        LeadingTone = 1 << 7,
    };

    class Chord : public Identifiable
    {
        DegreeFlags degree;
        Accidental accidental;
        NoteValue rootOffset;
        QualityFlags qualities;
    };
}
