#pragma once

#include "types.h"
#include "chord.h"
#include "constants.h"

namespace dryad
{
    struct ScaleOffsets
    {
        ScaleOffsets
        (
            dryad_note_value tonic = 0,
            dryad_note_value supertonic = major_second,
            dryad_note_value mediant = major_third,
            dryad_note_value subdominant = perfect_fourth,
            dryad_note_value dominant = perfect_fifth,
            dryad_note_value submediant = major_sixth,
            dryad_note_value leadindTone = major_seventh
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

        dryad_note_value offsets[7];
    };

    const ScaleOffsets MajorScaleOffsets = ScaleOffsets();
    const ScaleOffsets MinorNaturalOffsets = ScaleOffsets(0, 2, 3, 5, 7, 8, 10);
    const ScaleOffsets MinorHarmonicOffsets = ScaleOffsets(0, 2, 3, 5, 7, 8, 11);
    const ScaleOffsets MinorMelodicOffsets = ScaleOffsets(0, 2, 3, 5, 7, 9, 11);

    struct ScaleDegrees
    {
        ScaleDegrees
        (
            dryad_chord first = dryad_chord(C, dryad_degree::tonic, dryad_chord_quality::major | dryad_chord_quality::major_seventh),
            dryad_chord second = dryad_chord(D, dryad_degree::supertonic, dryad_chord_quality::minor | dryad_chord_quality::seventh),
            dryad_chord third = dryad_chord(E, dryad_degree::mediant, dryad_chord_quality::minor | dryad_chord_quality::seventh),
            dryad_chord fourth = dryad_chord(F, dryad_degree::subdominant, dryad_chord_quality::major | dryad_chord_quality::major_seventh),
            dryad_chord fifth = dryad_chord(G, dryad_degree::dominant, dryad_chord_quality::major | dryad_chord_quality::major_seventh),
            dryad_chord sixth = dryad_chord(A, dryad_degree::submediant, dryad_chord_quality::major | dryad_chord_quality::seventh),
            dryad_chord seventh = dryad_chord(B, dryad_degree::leading_tone, dryad_chord_quality::half_diminished)
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

        dryad_chord chords[7];
    };

    class dryad_scale
    {
    public:
        dryad_scale
        (
            ScaleOffsets offsets = ScaleOffsets(),
            ScaleOffsets descendingOffsets = ScaleOffsets(),
            ScaleDegrees degrees = ScaleDegrees(),
            dryad_note_value root = C
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
                degrees.chords[i].root = (root + offsets.offsets[i]) % octave;
            }
        }

        static unsigned DegreeToScaleIndex(dryad_degree degree)
        {
            switch(degree)
            {
            case dryad_degree::tonic: return 0;
            case dryad_degree::supertonic: return 1;
            case dryad_degree::mediant: return 2;
            case dryad_degree::subdominant: return 3;
            case dryad_degree::dominant: return 4;
            case dryad_degree::submediant: return 5;
            case dryad_degree::leading_tone: return 6;
            default:
                return 7;
            }
        }

        dryad_note_value get_degree_root(dryad_degree degree) const
        {
            unsigned degreeIndex = DegreeToScaleIndex(degree);
            if (degreeIndex < 7)
            {
                return degrees.chords[degreeIndex].root;
            }
            return octave;
        }

        ScaleOffsets offsets;
        ScaleOffsets descendingOffsets;
        ScaleDegrees degrees;
        dryad_note_value root;

        const dryad_chord& TonicChord() const { return degrees.chords[0]; }
        const dryad_chord& SupertonicChord() const { return degrees.chords[1]; }
        const dryad_chord& MediantChord() const { return degrees.chords[2]; }
        const dryad_chord& SubdominantChord() const { return degrees.chords[3]; }
        const dryad_chord& DominantChord() const { return degrees.chords[4]; }
        const dryad_chord& SubmediantChord() const { return degrees.chords[5]; }
        const dryad_chord& LeadingToneChord() const { return degrees.chords[6]; }
        const dryad_chord SecondaryDominantChord() const
        {
            return dryad_chord(DominantChord().root + perfect_fifth, dryad_degree::dominant, dryad_chord_quality::major | dryad_chord_quality::major_seventh);
        }
    };

    const dryad_scale MajorScale = dryad_scale();
}
