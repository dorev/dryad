#pragma once

#include "chord.h"
#include "graph.h"

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

    inline Error getChordOffsetsFromRoot(const Chord& chord, const Scale* scale, Vector<NoteRelative>& offsets)
    {
        if (!scale)
            return InvalidScale;

        if (chord.degree == Degree::Invalid)
            return InvalidDegree;

        Degree degree = chord.degree;

        ChordQuality qualities = scale->degreeQualities[degree];
        unsigned qualitiesBits = static_cast<unsigned>(qualities);
        unsigned chordBits = static_cast<unsigned>(chord.qualities);
        unsigned baseMask = 0x7;

        if (chordBits & baseMask)
            qualitiesBits = (qualitiesBits & ~baseMask) | (chordBits & baseMask);

        qualitiesBits |= (chordBits & ~baseMask);
        qualities = static_cast<ChordQuality>(qualitiesBits);

        NoteRelative degreeOffsetFromRoot = scale->noteOffsets[degree];

        offsets.clear();

        Vector<NoteRelative> intervals;
        intervals.push_back(0);

        unsigned baseQuality = qualitiesBits & baseMask;

        switch (baseQuality)
        {
        case static_cast<unsigned>(ChordQuality::Minor):
            intervals.push_back(3);
            intervals.push_back(7);
            break;
        case static_cast<unsigned>(ChordQuality::Major):
            intervals.push_back(4);
            intervals.push_back(7);
            break;
        case static_cast<unsigned>(ChordQuality::HalfDiminished):
            intervals.push_back(3);
            intervals.push_back(6);
            intervals.push_back(10);
            break;
        case static_cast<unsigned>(ChordQuality::Augmented):
            intervals.push_back(4);
            intervals.push_back(8);
            break;
        case static_cast<unsigned>(ChordQuality::Sus2):
            intervals.push_back(2);
            intervals.push_back(7);
            break;
        case static_cast<unsigned>(ChordQuality::Sus4):
            intervals.push_back(5);
            intervals.push_back(7);
            break;
        case static_cast<unsigned>(ChordQuality::Diminished):
            intervals.push_back(3);
            intervals.push_back(6);
            break;
        default:
            break;
        }

        auto pushUnique = [&](NoteRelative interval)
        {
            for (NoteRelative value : intervals)
            {
                if (value == interval)
                return;
            }

            intervals.push_back(interval);
        };

        if (bool(qualities & ChordQuality::SeventhMinor))
            pushUnique(10);
        if (bool(qualities & ChordQuality::SeventhMajor))
            pushUnique(11);
        if (bool(qualities & ChordQuality::Add6))
            pushUnique(9);
        if (bool(qualities & ChordQuality::Add9))
            pushUnique(14);
        if (bool(qualities & ChordQuality::Add11))
            pushUnique(17);
        if (bool(qualities & ChordQuality::Add13))
            pushUnique(21);

        auto alterInterval = [&](NoteRelative interval, int delta) -> bool
        {
            for (NoteRelative& value : intervals)
            {
                if (value == interval)
                {
                    value += delta;
                    return true;
                }
            }
            return false;
        };

        auto alterBase = [&](NoteRelative interval, int delta) -> bool
        {
            if (alterInterval(interval, delta))
                return true;

            return alterInterval(interval + Octave, delta);
        };

        if (bool(qualities & ChordQuality::Flat2))
            alterBase(2, -1);
        if (bool(qualities & ChordQuality::Flat3))
        {
            alterBase(4, -1);
            alterBase(3, -1);
        }
        if (bool(qualities & ChordQuality::Flat4))
            alterBase(5, -1);
        if (bool(qualities & ChordQuality::Flat5))
        {
            if (!alterBase(7, -1))
                alterBase(8, -1);
        }
        if (bool(qualities & ChordQuality::Flat6))
        {
            if (!alterBase(9, -1))
                alterBase(8, -1);
        }
        if (bool(qualities & ChordQuality::Flat7))
        {
            if (!alterBase(11, -1))
                alterBase(10, -1);
        }
        if (bool(qualities & ChordQuality::Flat9))
            alterInterval(14, -1);
        if (bool(qualities & ChordQuality::Flat11))
            alterInterval(17, -1);
        if (bool(qualities & ChordQuality::Flat13))
            alterInterval(21, -1);
        if (bool(qualities & ChordQuality::Sharp2))
            alterBase(2, 1);
        if (bool(qualities & ChordQuality::Sharp3))
        {
            alterBase(3, 1);
            alterBase(4, 1);
        }
        if (bool(qualities & ChordQuality::Sharp4))
            alterBase(5, 1);
        if (bool(qualities & ChordQuality::Sharp5))
        {
            if (!alterBase(7, 1))
                alterBase(6, 1);
        }
        if (bool(qualities & ChordQuality::Sharp6))
        {
            if (!alterBase(8, 1))
                alterBase(9, 1);
        }
        if (bool(qualities & ChordQuality::Sharp7))
        {
            if (!alterBase(10, 1))
                alterBase(11, 1);
        }
        if (bool(qualities & ChordQuality::Sharp9))
            alterInterval(14, 1);
        if (bool(qualities & ChordQuality::Sharp11))
            alterInterval(17, 1);
        if (bool(qualities & ChordQuality::Sharp13))
            alterInterval(21, 1);

        if (chord.accidental == Accidental::Flat)
            degreeOffsetFromRoot--;
        else if (chord.accidental == Accidental::Sharp)
            degreeOffsetFromRoot++;

        for (NoteRelative interval : intervals)
            offsets.push_back(degreeOffsetFromRoot + interval);

        return Success;
    }

} // namespace Dryad
