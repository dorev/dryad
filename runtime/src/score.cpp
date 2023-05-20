#include "score.h"

namespace Dryad
{
    Score::Score()
        : _info(0.0f)
        , _harmonicFrames(InitialHarmonicFramesCount)
        , _events()
    {
    }

    void Score::Reset(Time startTime)
    {
        _info = ScoreInfo(startTime);
        _harmonicFrames.Reset(InitialHarmonicFramesCount);
        _events.Clean();
    }

    Result Score::UpdateHarmony(Interlude* interlude, HarmonicTransition& harmonicTransition)
    {
        // Evaluate how much time we have to insert the next harmonic frame
        // Split or truncate the current frame depending on the situation
        // THE OBJECTIVE IS THAT IT SOUNDS NICE, NOT THAT WE HAVE 67789 OPTIONS!!!

        // If we have an interlude, we insert it right away
        // If we have an interlude AND an harmonic transition, pushFront the interlude and then insert
        // the transition based on the next harmonic frame

        return Result::NotYetImplemented;
    }

    Result Score::UpdateMotifs(Map<Motif*, Int32>& motifVariations)
    {
        return Result::NotYetImplemented;
    }

    Result Score::UpdateTempo(TempoChange& tempoChange)
    {
        return Result::NotYetImplemented;
    }

    Result Score::Commit(Time deltaTime, Vector<ScoreEvent>& newCommittedEvents)
    {
        return Result::NotYetImplemented;
    }
}
