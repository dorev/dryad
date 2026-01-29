#pragma once

#include "chord.h"
#include "graph.h"
#include "constants.h"

namespace Dryad
{

    struct ScaleNoteOffsets
    {
        ScaleNoteOffsets
        (
            NoteRelative tonic,
            NoteRelative supertonic,
            NoteRelative mediant,
            NoteRelative subdominant,
            NoteRelative dominant,
            NoteRelative submediant,
            NoteRelative leadingTone
        );

        inline NoteRelative operator[](Degree degree) const
        {
            if (degree >= Degree::Tonic && degree < Degree::Limit)
                return degrees[static_cast<int>(degree)];

            return static_cast<NoteRelative>(Invalid);
        }

        NoteRelative degrees[7];
    };

    struct ScaleDegreeQualities
    {
        ScaleDegreeQualities
        (
            ChordQuality tonic,
            ChordQuality supertonic,
            ChordQuality mediant,
            ChordQuality subdominant,
            ChordQuality dominant,
            ChordQuality submediant,
            ChordQuality leadingTone
        );

        inline ChordQuality operator[](Degree degree) const
        {
            if (degree >= Degree::Tonic && degree < Degree::Limit)
                return degrees[static_cast<int>(degree)];

            return ChordQuality::Invalid;
        }

        ChordQuality degrees[7];
    };

    class Scale : public Node
    {
    public:
        DRYAD_NODE_CLASS_ID(Scale);

        Scale(ScaleNoteOffsets noteOffsets, ScaleDegreeQualities degreeQualities);

        Scale(const Scale& other);

        Scale& operator=(const Scale& other);

        NoteRelative getDegreeNoteOffset(Degree degree);
        ChordQuality getDegreeChordQuality(Degree degree);

        ScaleNoteOffsets noteOffsets;
        ScaleDegreeQualities degreeQualities;
    };

    struct ScaleLibrary
    {
        inline static const ScaleNoteOffsets IonianOffsets{0, 2, 4, 5, 7, 9, 11};
        inline static const ScaleNoteOffsets MajorOffsets = IonianOffsets;
        inline static const ScaleNoteOffsets DorianOffsets{0, 2, 3, 5, 7, 9, 10};
        inline static const ScaleNoteOffsets PhrygianOffsets{0, 1, 3, 5, 7, 8, 10};
        inline static const ScaleNoteOffsets LydianOffsets{0, 2, 4, 6, 7, 9, 11};
        inline static const ScaleNoteOffsets MixolydianOffsets{0, 2, 4, 5, 7, 9, 10};
        inline static const ScaleNoteOffsets AeolianOffsets{0, 2, 3, 5, 7, 8, 10};
        inline static const ScaleNoteOffsets MinorNaturalOffsets = AeolianOffsets;
        inline static const ScaleNoteOffsets LocrianOffsets{0, 1, 3, 5, 6, 8, 10};
        inline static const ScaleNoteOffsets MinorHarmonicOffsets{0, 2, 3, 5, 7, 8, 11};
        inline static const ScaleNoteOffsets MinorMelodicOffsets{0, 2, 3, 5, 7, 9, 11};

        inline static const ScaleDegreeQualities IonianDegreeQualities
        {
            ChordQuality::Major | ChordQuality::SeventhMajor,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::Major | ChordQuality::SeventhMajor,
            ChordQuality::Major | ChordQuality::SeventhMinor,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::HalfDiminished
        };
        
        inline static const ScaleDegreeQualities MajorDegreeQualities = IonianDegreeQualities;

        inline static const ScaleDegreeQualities DorianDegreeQualities
        {
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::Major | ChordQuality::SeventhMajor,
            ChordQuality::Major | ChordQuality::SeventhMinor,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::HalfDiminished,
            ChordQuality::Major | ChordQuality::SeventhMajor
        };

        inline static const ScaleDegreeQualities PhrygianDegreeQualities
        {
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::Major | ChordQuality::SeventhMinor,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::HalfDiminished,
            ChordQuality::Major | ChordQuality::SeventhMajor,
            ChordQuality::Minor | ChordQuality::SeventhMinor
        };

        inline static const ScaleDegreeQualities LydianDegreeQualities
        {
            ChordQuality::Major | ChordQuality::SeventhMajor,
            ChordQuality::Major | ChordQuality::SeventhMinor,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::HalfDiminished,
            ChordQuality::Major | ChordQuality::SeventhMajor,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::Major | ChordQuality::SeventhMinor
        };

        inline static const ScaleDegreeQualities MixolydianDegreeQualities
        {
            ChordQuality::Major | ChordQuality::SeventhMinor,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::HalfDiminished,
            ChordQuality::Major | ChordQuality::SeventhMajor,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::Major | ChordQuality::SeventhMinor,
            ChordQuality::Minor | ChordQuality::SeventhMinor
        };

        inline static const ScaleDegreeQualities AeolianDegreeQualities
        {
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::HalfDiminished,
            ChordQuality::Major | ChordQuality::SeventhMajor,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::Major | ChordQuality::SeventhMajor,
            ChordQuality::Major | ChordQuality::SeventhMinor
        };

        inline static const ScaleDegreeQualities MinorNaturalDegreeQualities = AeolianDegreeQualities;

        inline static const ScaleDegreeQualities LocrianDegreeQualities
        {
            ChordQuality::HalfDiminished,
            ChordQuality::Major | ChordQuality::SeventhMajor,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::Major | ChordQuality::SeventhMinor,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::Minor | ChordQuality::SeventhMinor
        };

        inline static const ScaleDegreeQualities MinorHarmonicDegreeQualities
        {
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::Diminished,
            ChordQuality::Augmented,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::Major | ChordQuality::SeventhMinor,
            ChordQuality::Major | ChordQuality::SeventhMajor,
            ChordQuality::Diminished
        };

        inline static const ScaleDegreeQualities MinorMelodicDegreeQualities
        {
            ChordQuality::Minor | ChordQuality::SeventhMajor,
            ChordQuality::Minor | ChordQuality::SeventhMinor,
            ChordQuality::Augmented,
            ChordQuality::Major | ChordQuality::SeventhMajor,
            ChordQuality::Major | ChordQuality::SeventhMinor,
            ChordQuality::Diminished,
            ChordQuality::Diminished
        };

        inline static const Scale IonianScale{IonianOffsets, IonianDegreeQualities};
        inline static const Scale MajorScale{IonianScale};
        inline static const Scale DorianScale{DorianOffsets, DorianDegreeQualities};
        inline static const Scale PhrygianScale{PhrygianOffsets, PhrygianDegreeQualities};
        inline static const Scale LydianScale{LydianOffsets, LydianDegreeQualities};
        inline static const Scale MixolydianScale{MixolydianOffsets, MixolydianDegreeQualities};
        inline static const Scale AeolianScale{AeolianOffsets, AeolianDegreeQualities};
        inline static const Scale MinorNaturalScale{AeolianScale};
        inline static const Scale LocrianScale{LocrianOffsets, LocrianDegreeQualities};
        inline static const Scale MinorHarmonicScale{MinorHarmonicOffsets, MinorHarmonicDegreeQualities};
        inline static const Scale MinorMelodicScale{MinorMelodicOffsets, MinorMelodicDegreeQualities};
    };

} // namespace Dryad
