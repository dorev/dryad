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
            if (degree >= Degree::tonic && degree < Degree::limit)
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
            if (degree >= Degree::tonic && degree < Degree::limit)
                return degrees[static_cast<int>(degree)];

            return ChordQuality::invalid;
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
        inline static const ScaleNoteOffsets ionianOffsets{0, 2, 4, 5, 7, 9, 11};
        inline static const ScaleNoteOffsets majorOffsets = ionianOffsets;
        inline static const ScaleNoteOffsets dorianOffsets{0, 2, 3, 5, 7, 9, 10};
        inline static const ScaleNoteOffsets phrygianOffsets{0, 1, 3, 5, 7, 8, 10};
        inline static const ScaleNoteOffsets lydianOffsets{0, 2, 4, 6, 7, 9, 11};
        inline static const ScaleNoteOffsets mixolydianOffsets{0, 2, 4, 5, 7, 9, 10};
        inline static const ScaleNoteOffsets aeolianOffsets{0, 2, 3, 5, 7, 8, 10};
        inline static const ScaleNoteOffsets minorNaturalOffsets = aeolianOffsets;
        inline static const ScaleNoteOffsets locrianOffsets{0, 1, 3, 5, 6, 8, 10};
        inline static const ScaleNoteOffsets minorHarmonicOffsets{0, 2, 3, 5, 7, 8, 11};
        inline static const ScaleNoteOffsets minorMelodicOffsets{0, 2, 3, 5, 7, 9, 11};

        inline static const ScaleDegreeQualities ionianDegreeQualities
        {
            ChordQuality::major | ChordQuality::seventhMajor,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::major | ChordQuality::seventhMajor,
            ChordQuality::major | ChordQuality::seventhMinor,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::halfDiminished
        };
        
        inline static const ScaleDegreeQualities majorDegreeQualities = ionianDegreeQualities;

        inline static const ScaleDegreeQualities dorianDegreeQualities
        {
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::major | ChordQuality::seventhMajor,
            ChordQuality::major | ChordQuality::seventhMinor,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::halfDiminished,
            ChordQuality::major | ChordQuality::seventhMajor
        };

        inline static const ScaleDegreeQualities phrygianDegreeQualities
        {
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::major | ChordQuality::seventhMinor,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::halfDiminished,
            ChordQuality::major | ChordQuality::seventhMajor,
            ChordQuality::minor | ChordQuality::seventhMinor
        };

        inline static const ScaleDegreeQualities lydianDegreeQualities
        {
            ChordQuality::major | ChordQuality::seventhMajor,
            ChordQuality::major | ChordQuality::seventhMinor,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::halfDiminished,
            ChordQuality::major | ChordQuality::seventhMajor,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::major | ChordQuality::seventhMinor
        };

        inline static const ScaleDegreeQualities mixolydianDegreeQualities
        {
            ChordQuality::major | ChordQuality::seventhMinor,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::halfDiminished,
            ChordQuality::major | ChordQuality::seventhMajor,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::major | ChordQuality::seventhMinor,
            ChordQuality::minor | ChordQuality::seventhMinor
        };

        inline static const ScaleDegreeQualities aeolianDegreeQualities
        {
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::diminished,
            ChordQuality::major | ChordQuality::seventhMajor,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::major | ChordQuality::seventhMajor,
            ChordQuality::major | ChordQuality::seventhMinor
        };

        inline static const ScaleDegreeQualities minorNaturalDegreeQualities = aeolianDegreeQualities;

        inline static const ScaleDegreeQualities locrianDegreeQualities
        {
            ChordQuality::halfDiminished,
            ChordQuality::major | ChordQuality::seventhMajor,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::major | ChordQuality::seventhMinor,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::minor | ChordQuality::seventhMinor
        };

        inline static const ScaleDegreeQualities minorHarmonicDegreeQualities
        {
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::diminished,
            ChordQuality::augmented,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::major | ChordQuality::seventhMinor,
            ChordQuality::major | ChordQuality::seventhMajor,
            ChordQuality::diminished
        };

        inline static const ScaleDegreeQualities minorMelodicDegreeQualities
        {
            ChordQuality::minor | ChordQuality::seventhMajor,
            ChordQuality::minor | ChordQuality::seventhMinor,
            ChordQuality::augmented,
            ChordQuality::major | ChordQuality::seventhMajor,
            ChordQuality::major | ChordQuality::seventhMinor,
            ChordQuality::diminished,
            ChordQuality::diminished
        };

        inline static const Scale ionian_scale{ionianOffsets, ionianDegreeQualities};
        inline static const Scale major_scale{ionian_scale};
        inline static const Scale dorian_scale{dorianOffsets, dorianDegreeQualities};
        inline static const Scale phrygian_scale{phrygianOffsets, phrygianDegreeQualities};
        inline static const Scale lydian_scale{lydianOffsets, lydianDegreeQualities};
        inline static const Scale mixolydian_scale{mixolydianOffsets, mixolydianDegreeQualities};
        inline static const Scale aeolian_scale{aeolianOffsets, aeolianDegreeQualities};
        inline static const Scale minor_natural_scale{aeolian_scale};
        inline static const Scale locrian_scale{locrianOffsets, locrianDegreeQualities};
        inline static const Scale minor_harmonic_scale{minorHarmonicOffsets, minorHarmonicDegreeQualities};
        inline static const Scale minor_melodic_scale{minorMelodicOffsets, minorMelodicDegreeQualities};
    };

    inline Error getChordOffsetsFromRoot(const Chord& chord, const Scale* scale, Vector<NoteRelative>& offsets)
    {
        if (!scale)
            return InvalidScale;

        if (chord.degree == Degree::invalid)
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
        case static_cast<unsigned>(ChordQuality::minor):
            intervals.push_back(3);
            intervals.push_back(7);
            break;
        case static_cast<unsigned>(ChordQuality::major):
            intervals.push_back(4);
            intervals.push_back(7);
            break;
        case static_cast<unsigned>(ChordQuality::halfDiminished):
            intervals.push_back(3);
            intervals.push_back(6);
            intervals.push_back(10);
            break;
        case static_cast<unsigned>(ChordQuality::augmented):
            intervals.push_back(4);
            intervals.push_back(8);
            break;
        case static_cast<unsigned>(ChordQuality::sus2):
            intervals.push_back(2);
            intervals.push_back(7);
            break;
        case static_cast<unsigned>(ChordQuality::sus4):
            intervals.push_back(5);
            intervals.push_back(7);
            break;
        case static_cast<unsigned>(ChordQuality::diminished):
            intervals.push_back(3);
            intervals.push_back(6);
            break;
        default:
            break;
        }

        auto pushUnique = [&](NoteRelative interval)
        {
            for (NoteRelative value : intervals)
                if (value == interval)
                    return;
            intervals.push_back(interval);
        };

        if (bool(qualities & ChordQuality::seventhMinor))
            pushUnique(10);
        if (bool(qualities & ChordQuality::seventhMajor))
            pushUnique(11);
        if (bool(qualities & ChordQuality::add6))
            pushUnique(9);
        if (bool(qualities & ChordQuality::add9))
            pushUnique(14);
        if (bool(qualities & ChordQuality::add11))
            pushUnique(17);
        if (bool(qualities & ChordQuality::add13))
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
            return alterInterval(interval + 12, delta);
        };

        if (bool(qualities & ChordQuality::flat2))
            alterBase(2, -1);
        if (bool(qualities & ChordQuality::flat3))
        {
            alterBase(4, -1);
            alterBase(3, -1);
        }
        if (bool(qualities & ChordQuality::flat4))
            alterBase(5, -1);
        if (bool(qualities & ChordQuality::flat5))
        {
            if (!alterBase(7, -1))
                alterBase(8, -1);
        }
        if (bool(qualities & ChordQuality::flat6))
        {
            if (!alterBase(9, -1))
                alterBase(8, -1);
        }
        if (bool(qualities & ChordQuality::flat7))
        {
            if (!alterBase(11, -1))
                alterBase(10, -1);
        }
        if (bool(qualities & ChordQuality::flat9))
            alterInterval(14, -1);
        if (bool(qualities & ChordQuality::flat11))
            alterInterval(17, -1);
        if (bool(qualities & ChordQuality::flat13))
            alterInterval(21, -1);
        if (bool(qualities & ChordQuality::sharp2))
            alterBase(2, 1);
        if (bool(qualities & ChordQuality::sharp3))
        {
            alterBase(3, 1);
            alterBase(4, 1);
        }
        if (bool(qualities & ChordQuality::sharp4))
            alterBase(5, 1);
        if (bool(qualities & ChordQuality::sharp5))
        {
            if (!alterBase(7, 1))
                alterBase(6, 1);
        }
        if (bool(qualities & ChordQuality::sharp6))
        {
            if (!alterBase(8, 1))
                alterBase(9, 1);
        }
        if (bool(qualities & ChordQuality::sharp7))
        {
            if (!alterBase(10, 1))
                alterBase(11, 1);
        }
        if (bool(qualities & ChordQuality::sharp9))
            alterInterval(14, 1);
        if (bool(qualities & ChordQuality::sharp11))
            alterInterval(17, 1);
        if (bool(qualities & ChordQuality::sharp13))
            alterInterval(21, 1);

        if (chord.accidental == Accidental::flat)
            degreeOffsetFromRoot--;
        else if (chord.accidental == Accidental::sharp)
            degreeOffsetFromRoot++;

        for (NoteRelative interval : intervals)
            offsets.push_back(degreeOffsetFromRoot + interval);

        return Success;
    }

} // namespace Dryad
