#include "chord.h"
#include "scale.h"

namespace Dryad
{
    Chord::Chord
    (
        NoteValue root,
        Degree degree,
        ChordQualities qualities,
        Accidental accidental
    )
        : root(root)
        , degree(degree)
        , qualities(qualities)
        , accidental(accidental)
    {
    }

    bool Chord::operator==(const Chord& other) const
    {
        return root == other.root
            && degree == other.degree
            && qualities == other.qualities
            && accidental == other.accidental;
    }

    bool Chord::IsDominantOf(Chord other) const
    {
        if (FlagIsSet(qualities, ChordQualities::Major))
        {
            other.root += PerfectFifth;
            other.root %= Octave;
            return AreSimilar(*this, other);
        }
        return false;
    }

    bool Chord::AreSimilar(const Chord& left, const Chord& right)
    {
        ChordQualities leftQualitiesSimplified = left.qualities & SimplifiedChordQualities;
        ChordQualities rightQualitiesSimplified = right.qualities & SimplifiedChordQualities;
        return left.root == right.root
            && left.accidental == right.accidental
            && leftQualitiesSimplified == rightQualitiesSimplified;
    }

    Result Chord::ApplyScale(const Scale* scale)
    {
        if (scale == nullptr)
        {
            return Result::InvalidScale;
        }
        root = scale->GetDegreeRoot(degree);
        if (root == Octave)
        {
            return Result::InvalidDegree;
        }
        return Result::Success;
    }
}
