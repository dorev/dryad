#pragma once

#include "types.h"
#include "chord.h"

namespace Dryad
{

    constexpr BPM DefaultTempo = 120.0f;

    constexpr Time Whole = 96;
    constexpr Time HalfDotted = 72;
    constexpr Time Half = 48;
    constexpr Time QuarterDotted = 36;
    constexpr Time HalfTriplet = 32;
    constexpr Time Quarter = 24;
    constexpr Time EighthDotted = 18;
    constexpr Time QuarterTriplet = 16;
    constexpr Time Eighth = 12;
    constexpr Time EighthTriplet = 8;
    constexpr Time Sixteenth = 6;
    constexpr Time SixteenthTriplet = 4;
    constexpr Time ThirthySeconth = 3;
    constexpr Time ThirthySeconthTriplet = 2;
    constexpr Time DefaultBeatAlignment = Eighth;

    constexpr NoteValue MinorSecond = 1;
    constexpr NoteValue MajorSecond = 2;
    constexpr NoteValue MinorThird = 3;
    constexpr NoteValue MajorThird = 4;
    constexpr NoteValue PerfectFourth = 5;
    constexpr NoteValue Tritone = 6;
    constexpr NoteValue PerfectFifth = 7;
    constexpr NoteValue MinorSixth = 8;
    constexpr NoteValue MajorSixth = 9;
    constexpr NoteValue MinorSeventh = 10;
    constexpr NoteValue MajorSeventh = 11;
    constexpr NoteValue Octave = 12;

    constexpr int ScaleLimit = 12;
    constexpr int OctaveLimit = 11;
    constexpr int MiddleOctave = 4;
    constexpr int DegreesPerOctave = static_cast<int>(Degree::Limit) - 1;

    constexpr float Frequencies[ScaleLimit][OctaveLimit] =
    {
        { 16.3516f, 32.7032f, 65.4064f, 130.81f, 261.63f, 523.25f, 1046.5f, 2093.0f, 4186.0f, 8372.0f,  16744.0f },
        { 17.3239f, 34.6478f, 69.2957f, 138.59f, 277.18f, 554.37f, 1108.7f, 2217.5f, 4434.9f, 8869.8f,  17740.0f },
        { 18.3540f, 36.7081f, 73.4162f, 146.83f, 293.66f, 587.33f, 1174.7f, 2349.3f, 4698.6f, 9397.3f,  18795.0f },
        { 19.4454f, 38.8909f, 77.7817f, 155.56f, 311.13f, 622.25f, 1244.5f, 2489.0f, 4978.0f, 9956.1f,  19912.0f },
        { 20.6017f, 41.2034f, 82.4069f, 164.81f, 329.63f, 659.26f, 1318.5f, 2637.0f, 5274.0f, 10548.0f, 21096.0f },
        { 21.8268f, 43.6535f, 87.3071f, 174.61f, 349.23f, 698.46f, 1396.9f, 2793.8f, 5587.7f, 11175.0f, 22350.0f },
        { 23.1247f, 46.2493f, 92.4986f, 185.00f, 369.99f, 739.99f, 1479.9f, 2959.9f, 5919.9f, 11840.0f, 23680.0f },
        { 24.4997f, 48.9994f, 97.9989f, 196.00f, 392.00f, 784.00f, 1568.0f, 3136.0f, 6272.0f, 12544.0f, 25088.0f },
        { 25.9565f, 51.9131f, 103.826f, 207.65f, 415.30f, 830.61f, 1661.2f, 3322.4f, 6644.9f, 13290.0f, 26580.0f },
        { 27.5000f, 55.0000f, 110.000f, 220.00f, 440.00f, 880.00f, 1760.0f, 3520.0f, 7040.0f, 14080.0f, 28160.0f },
        { 29.1352f, 58.2705f, 116.541f, 233.08f, 466.16f, 932.33f, 1864.7f, 3729.3f, 7458.6f, 14917.0f, 29834.0f },
        { 30.8677f, 61.7354f, 123.471f, 246.94f, 493.88f, 987.77f, 1975.5f, 3940.0f, 7880.0f, 15760.0f, 31520.0f },
    };

    constexpr NoteValue notes[ScaleLimit][OctaveLimit] =
    {
        { 0,  12, 24, 36, 48, 60, 72, 84,  96, 108, 120 },
        { 1,  13, 25, 37, 49, 61, 73, 85,  97, 109, 121 },
        { 2,  14, 26, 38, 50, 62, 74, 86,  98, 110, 122 },
        { 3,  15, 27, 39, 51, 63, 75, 87,  99, 111, 123 },
        { 4,  16, 28, 40, 52, 64, 76, 88, 100, 112, 124 },
        { 5,  17, 29, 41, 53, 65, 77, 89, 101, 113, 125 },
        { 6,  18, 30, 42, 54, 66, 78, 90, 102, 114, 126 },
        { 7,  19, 31, 43, 55, 67, 79, 91, 103, 115, 127 },
        { 8,  20, 32, 44, 56, 68, 80, 92, 104, 116, 128 },
        { 9,  21, 33, 45, 57, 69, 81, 93, 105, 117, 129 },
        { 10, 22, 34, 46, 58, 70, 82, 94, 106, 118, 130 },
        { 11, 23, 35, 47, 59, 71, 83, 95, 107, 119, 131 },
    };

    constexpr NoteValue C = 0;
    constexpr NoteValue Cs = 1;
    constexpr NoteValue Db = 1;
    constexpr NoteValue D = 2;
    constexpr NoteValue Ds = 3;
    constexpr NoteValue Eb = 3;
    constexpr NoteValue E = 4;
    constexpr NoteValue F = 5;
    constexpr NoteValue Fs = 6;
    constexpr NoteValue Gb = 6;
    constexpr NoteValue G = 7;
    constexpr NoteValue Gs = 8;
    constexpr NoteValue Ab = 8;
    constexpr NoteValue A = 9;
    constexpr NoteValue As = 10;
    constexpr NoteValue Bb = 10;
    constexpr NoteValue B = 11;

    constexpr NoteValue MiddleC = notes[C][MiddleOctave];

    float constexpr MidiToFrequency(NoteValue midi)
    {
        int note = midi % 12;
        int octave = midi / 12 - 1;
        return Frequencies[note][octave];
    }

} // namespace Dryad