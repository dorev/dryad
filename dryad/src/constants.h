#pragma once

#include "types.h"

namespace dryad_constants
{
    constexpr dryad_bpm default_tempo = 120.0f;

    constexpr dryad_time whole = 96;
    constexpr dryad_time half_dotted = 72;
    constexpr dryad_time half = 48;
    constexpr dryad_time quarter_dotted = 36;
    constexpr dryad_time half_triplet = 32;
    constexpr dryad_time quarter = 24;
    constexpr dryad_time eighth_dotted = 18;
    constexpr dryad_time quarter_triplet = 16;
    constexpr dryad_time eighth = 12;
    constexpr dryad_time eighth_triplet = 8;
    constexpr dryad_time sixteenth = 6;
    constexpr dryad_time sixteenth_triplet = 4;
    constexpr dryad_time thirthy_seconth = 3;
    constexpr dryad_time thirthy_seconth_triplet = 2;
    constexpr dryad_time default_beat_alignement = eighth;

    constexpr dryad_note_value minor_second = 1;
    constexpr dryad_note_value major_second = 2;
    constexpr dryad_note_value minor_third = 3;
    constexpr dryad_note_value major_third = 4;
    constexpr dryad_note_value perfect_fourth = 5;
    constexpr dryad_note_value tritone = 6;
    constexpr dryad_note_value perfect_fifth = 7;
    constexpr dryad_note_value minor_sixth = 8;
    constexpr dryad_note_value major_sixth = 9;
    constexpr dryad_note_value minor_seventh = 10;
    constexpr dryad_note_value major_seventh = 11;
    constexpr dryad_note_value octave = 12;

    constexpr unsigned scale_limit = 12;
    constexpr unsigned octave_limit = 11;

    constexpr float frequencies[scale_limit][octave_limit] =
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

    constexpr dryad_note_value notes[scale_limit][octave_limit] =
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

    constexpr dryad_note_value C = 0;
    constexpr dryad_note_value Cs = 1;
    constexpr dryad_note_value Db = 1;
    constexpr dryad_note_value D = 2;
    constexpr dryad_note_value Ds = 3;
    constexpr dryad_note_value Eb = 3;
    constexpr dryad_note_value E = 4;
    constexpr dryad_note_value F = 5;
    constexpr dryad_note_value Fs = 6;
    constexpr dryad_note_value Gb = 6;
    constexpr dryad_note_value G = 7;
    constexpr dryad_note_value Gs = 8;
    constexpr dryad_note_value Ab = 8;
    constexpr dryad_note_value A = 9;
    constexpr dryad_note_value As = 10;
    constexpr dryad_note_value Bb = 10;
    constexpr dryad_note_value B = 11;

    constexpr dryad_note_value middle_c = notes[C][4];
}

    float constexpr dryad_midi_to_frequency(dryad_note_value midi)
    {
        int note = midi % 12;
        int octave = midi / 12 - 1;
        return dryad_constants::frequencies[note][octave];
    }
