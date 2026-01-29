#include "harmony.h"
#include "scale.h"

namespace Dryad
{

    Error getChordOffsetsFromRoot(const Chord& chord, const Scale* scale, Vector<NoteRelative>& offsets)
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

        auto ensureInterval = [&](NoteRelative interval)
        {
            pushUnique(interval);
        };

        auto alterOrAddInterval = [&](NoteRelative interval, int delta)
        {
            if (!alterInterval(interval, delta))
                ensureInterval(interval + delta);
        };

        auto baseDegreeInterval = [&](int degreeToAlter) -> NoteRelative
        {
            switch (degreeToAlter)
            {
            case 2: return 2;
            case 3: return 4;
            case 4: return 5;
            case 5: return 7;
            case 6: return 9;
            case 7: return 11;
            default: return Invalid;
            }
        };

        auto extensionInterval = [&](int extension) -> NoteRelative
        {
            switch (extension)
            {
            case 9: return 14;
            case 11: return 17;
            case 13: return 21;
            default: return Invalid;
            }
        };

        auto applyDegreeAlteration = [&](int degreeToAlter, int delta)
        {
            NoteRelative interval = baseDegreeInterval(degreeToAlter);
            if (interval == Invalid)
                return;

            alterOrAddInterval(interval, delta);
        };

        auto applyExtensionAlteration = [&](int extension, int delta)
        {
            NoteRelative interval = extensionInterval(extension);
            if (interval == Invalid)
                return;

            alterOrAddInterval(interval, delta);
        };

        if (bool(qualities & ChordQuality::Flat2))
            applyDegreeAlteration(2, -1);
        if (bool(qualities & ChordQuality::Flat3))
            applyDegreeAlteration(3, -1);
        if (bool(qualities & ChordQuality::Flat4))
            applyDegreeAlteration(4, -1);
        if (bool(qualities & ChordQuality::Flat5))
            applyDegreeAlteration(5, -1);
        if (bool(qualities & ChordQuality::Flat6))
            applyDegreeAlteration(6, -1);
        if (bool(qualities & ChordQuality::Flat7))
            applyDegreeAlteration(7, -1);
        if (bool(qualities & ChordQuality::Flat9))
            applyExtensionAlteration(9, -1);
        if (bool(qualities & ChordQuality::Flat11))
            applyExtensionAlteration(11, -1);
        if (bool(qualities & ChordQuality::Flat13))
            applyExtensionAlteration(13, -1);
        if (bool(qualities & ChordQuality::Sharp2))
            applyDegreeAlteration(2, 1);
        if (bool(qualities & ChordQuality::Sharp3))
            applyDegreeAlteration(3, 1);
        if (bool(qualities & ChordQuality::Sharp4))
            applyDegreeAlteration(4, 1);
        if (bool(qualities & ChordQuality::Sharp5))
            applyDegreeAlteration(5, 1);
        if (bool(qualities & ChordQuality::Sharp6))
            applyDegreeAlteration(6, 1);
        if (bool(qualities & ChordQuality::Sharp7))
            applyDegreeAlteration(7, 1);
        if (bool(qualities & ChordQuality::Sharp9))
            applyExtensionAlteration(9, 1);
        if (bool(qualities & ChordQuality::Sharp11))
            applyExtensionAlteration(11, 1);
        if (bool(qualities & ChordQuality::Sharp13))
            applyExtensionAlteration(13, 1);

        if (chord.accidental == Accidental::Flat)
            degreeOffsetFromRoot--;
        else if (chord.accidental == Accidental::Sharp)
            degreeOffsetFromRoot++;

        for (NoteRelative interval : intervals)
            offsets.push_back(degreeOffsetFromRoot + interval);

        return Success;
    }

} // namespace Dryad
