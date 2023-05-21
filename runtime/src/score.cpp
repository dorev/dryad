#include "score.h"

namespace Dryad
{
    Score::Score()
        : _info(0.0f)
        , _harmonicFrames(DefaultHarmonicFramesCount)
        , _events()
    {
    }

    void Score::Reset(Time startTime)
    {
        _info = ScoreInfo(startTime);
        _harmonicFrames.Reset(DefaultHarmonicFramesCount);
        _events.Clean();
    }

    Result Score::UpdateHarmony(HarmonicTransition& harmonicTransition)
    {
        // Based on the transition time limit...
        //  * check if we can move toward an exit node
        //  * otherwise proceed to the next graph a the closest node finish point
        // ... cute strategies will follow later!!

        // In the case of a scale change, try to follow common chords or to switch
        // after the target scale dominant

        // Skip edge modulation when changing scale or graph

        return Result::NotYetImplemented;
    }

    Result Score::UpdateMotifs(Map<Motif*, Int32>& motifVariations)
    {
        // This will essentially be about updating the motif levels of the
        // current harmonic frame!
        return Result::NotYetImplemented;
    }

    Result Score::UpdateTempo(TempoChange& tempoChange)
    {
        // This will lead to the insertion of a ScoreEvent and also to some modification
        // of the harmonic frames (depending on the duration of the tempo change...)
        // I'm thinking about pushing this for a farther development cycle but I feel it
        // would be better to consider it sooner than later
        return Result::NotYetImplemented;
    }

    Result Score::Commit(Time deltaTime, Vector<ScoreEvent>& newCommittedEvents)
    {
        // This should be pretty straight-forward, just about scanning from the current
        // score position, toggling the notes bools and updating the vector
        return Result::NotYetImplemented;
    }
}
