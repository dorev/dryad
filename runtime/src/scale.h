#pragma once

#include "types.h"
#include "chord.h"
#include "constants.h"

namespace Dryad
{
    struct ScaleOffsets
    {
        ScaleOffsets
        (
            NoteValue tonic = 0,
            NoteValue supertonic = MajorSecond,
            NoteValue mediant = MajorThird,
            NoteValue subdominant = PerfectFourth,
            NoteValue dominant = PerfectFifth,
            NoteValue submediant = MajorSixth,
            NoteValue leadindTone = MajorSeventh
        )
            : offsets
            {
                tonic,
                supertonic,
                mediant,
                subdominant,
                dominant,
                submediant,
                leadindTone
            }
        {
        }

        NoteValue offsets[7];
    };

    const ScaleOffsets MajorScaleOffsets = ScaleOffsets();
    const ScaleOffsets MinorNaturalOffsets = ScaleOffsets(0, 2, 3, 5, 7, 8, 10);
    const ScaleOffsets MinorHarmonicOffsets = ScaleOffsets(0, 2, 3, 5, 7, 8, 11);
    const ScaleOffsets MinorMelodicOffsets = ScaleOffsets(0, 2, 3, 5, 7, 9, 11);

    struct ScaleDegrees
    {
        ScaleDegrees
        (
            Chord first = Chord(C, Degree::Tonic, ChordQualities::Major | ChordQualities::MajorSeventh),
            Chord second = Chord(D, Degree::Supertonic, ChordQualities::Minor | ChordQualities::Seventh),
            Chord third = Chord(E, Degree::Mediant, ChordQualities::Minor | ChordQualities::Seventh),
            Chord fourth = Chord(F, Degree::Subdominant, ChordQualities::Major | ChordQualities::MajorSeventh),
            Chord fifth = Chord(G, Degree::Dominant, ChordQualities::Major | ChordQualities::MajorSeventh),
            Chord sixth = Chord(A, Degree::Submediant, ChordQualities::Major | ChordQualities::Seventh),
            Chord seventh = Chord(B, Degree::LeadingTone, ChordQualities::HalfDiminished)
        )
            : chords
            {
                first,
                second,
                third,
                fourth,
                fifth,
                sixth,
                seventh
            }
        {
        }

        Chord chords[7];
    };

    class Scale
    {
    public:
        Scale
        (
            ScaleOffsets offsets = ScaleOffsets(),
            ScaleOffsets descendingOffsets = ScaleOffsets(),
            ScaleDegrees degrees = ScaleDegrees(),
            NoteValue root = C
        )
            : offsets(offsets)
            , descendingOffsets(descendingOffsets)
            , degrees(degrees)
            , root(root)
        {
            UpdateChordRoots();
        }

        void UpdateChordRoots()
        {
            for (int i = 0; i < 7; ++i)
            {
                degrees.chords[i].root = (root + offsets.offsets[i]) % Octave;
            }
        }

        ScaleOffsets offsets;
        ScaleOffsets descendingOffsets;
        ScaleDegrees degrees;
        NoteValue root;

        const Chord TonicChord() const { return degrees.chords[0]; }
        const Chord SupertonicChord() const { return degrees.chords[1]; }
        const Chord MediantChord() const { return degrees.chords[2]; }
        const Chord SubdominantChord() const { return degrees.chords[3]; }
        const Chord DominantChord() const { return degrees.chords[4]; }
        const Chord SubmediantChord() const { return degrees.chords[5]; }
        const Chord LeadingToneChord() const { return degrees.chords[6]; }
        const Chord SecondaryDominantChord() const
        {
            return Chord(DominantChord().root + PerfectFifth, Degree::Dominant, ChordQualities::Major | ChordQualities::MajorSeventh);
        }
    };

    const Scale MajorScale = Scale();
}
