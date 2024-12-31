
namespace dryad
{
    /*
    dryad_chord::dryad_chord
    (
        dryad_note_value root,
        dryad_degree degrees,
        dryad_chord_quality qualities,
        dryad_accidental accidental
    )
        : root(root)
        , degrees(degrees)
        , qualities(qualities)
        , accidental(accidental)
    {
    }

    bool dryad_chord::operator==(const dryad_chord& other) const
    {
        return root == other.root
            && degrees == other.degrees
            && qualities == other.qualities
            && accidental == other.accidental;
    }

    bool dryad_chord::is_dominant_of(dryad_chord other) const
    {
        if (bool (qualities & dryad_chord_quality::major))
        {
            other.root += perfect_fifth;
            other.root %= octave;
            return are_similar(*this, other);
        }
        return false;
    }

    bool dryad_chord::are_similar(const dryad_chord& left, const dryad_chord& right)
    {
        dryad_chord_quality leftQualitiesSimplified = left.qualities & simlified_chord_qualities;
        dryad_chord_quality rightQualitiesSimplified = right.qualities & simlified_chord_qualities;
        return left.root == right.root
            && left.accidental == right.accidental
            && leftQualitiesSimplified == rightQualitiesSimplified;
    }

    Result dryad_chord::apply_scale(const dryad_scale* scale)
    {
        if (scale == nullptr)
        {
            return Result::InvalidScale;
        }
        root = scale->get_degree_root(degrees);
        if (root == octave)
        {
            return Result::InvalidDegree;
        }
        return Result::NotImplementedYet;
    }
    */
}
