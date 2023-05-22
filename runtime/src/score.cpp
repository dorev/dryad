#include "score.h"

#include "graph.h"
#include "edge.h"
#include "node.h"
#include "random.h"

namespace Dryad
{
    Score::Score()
        : _ledger(0.0f, DefaultTempo)
        , _harmonicFrames(DefaultHarmonicFramesCount)
    {
    }

    void Score::Reset(Time startTime, Tempo startTempo)
    {
        _ledger = ScoreLedger(startTime, startTempo);
        _harmonicFrames.Reset(DefaultHarmonicFramesCount);
        //_events.Clean(); TODO: Clean ledger events
    }

    Result Score::UpdateHarmony(HarmonicTransition& transition)
    {
        if(_harmonicFrames.Empty())
        {
            return InitialHarmonicFrame(transition);
        }
        // With just a scale
        if(transition.graph == nullptr && transition.scale != nullptr)
        {
            return UpdateHarmonyScale(transition);
        }
        // With a graph and maybe a scale
        else if(transition.graph != nullptr)
        {
            // Plan a graph change, including a potential modulation
            return UpdateHarmonyGraph(transition);
        }
        else
        {
            // No scale or graph provided, nothing to do
            return Result::UselessCall;
        }
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
        // If we reach the end of a harmonic frame, prepare the next one if necessary.
        // Keep always t
        return Result::NotYetImplemented;
    }

    HarmonicFrame& Score::CurrentHarmonicFrame()
    {
        return _harmonicFrames.Front();
    }
    
    const HarmonicFrame& Score::CurrentHarmonicFrame() const
    {
        return _harmonicFrames.Front();
    }

    Tempo Score::CurrentTempo() const
    {
        if(_harmonicFrames.Empty())
        {
            return _ledger.startTempo;
        }
        return CurrentHarmonicFrame().tempo;
    }

    Scale* Score::CurrentScale() const
    {
        if(_harmonicFrames.Empty())
        {
            return nullptr;
        }
        return CurrentHarmonicFrame().scale;
    }

    Node* Score::CurrentNode()
    {
        if(_harmonicFrames.Empty())
        {
            return nullptr;
        }
        return CurrentHarmonicFrame().node;
    }

    Result Score::InitialHarmonicFrame(HarmonicTransition& transition)
    {
        Result result = Result:: EmptyResult;
        Graph* graph = transition.graph;
        Edge* entryEdge = transition.entryEdge;
        Scale* scale = transition.scale;

        // Validate the entry edge
        if(graph != nullptr)
        {
            // Validate that the provided edge belongs to the graph
            if(entryEdge != nullptr && !graph->HasEntryEdge(entryEdge))
            {
                return Result::EdgeNotFound;
            }
            // Or select randomly an entry edge
            else
            {
                result = RandomFrom(graph->entryEdges, entryEdge);
                if(result != Result::Success)
                {
                    return result;
                }
                ResetResult(result);
            }
        }
        else if(entryEdge == nullptr)
        {
            // No graph and no edge provided
            return Result::EdgeNotFound;
        }

        // Validate the entry node
        Node* node = entryEdge->destination;
        if(node == nullptr)
        {
            return Result::NodeNotFound;
        }
        if(node->graph == nullptr)
        {
            return Result::GraphNotFound;
        }

        // Initialize with default frame
        HarmonicFrame frame;
        frame.tempo = CurrentTempo();
        if(scale != nullptr)
        {
            frame.scale = scale;
            // Intentionally ignoring edge modulation
        }

        // Complete frame setup and add queue it
        return result = frame.MatchNode(node);
    }

    Result Score::UpdateHarmonyScale(HarmonicTransition& transition)
    {
        if(transition.maxDuration.IsZero()/*  || _events.Empty() _ledger.Empty!*/)
        {
            CurrentHarmonicFrame().scale = transition.scale;
            return Result::Success;
        }

        // Based on the transition duration, try to find an exit node,
        // or the dominant chord of the target scale
        ScoreTime duration = transition.maxDuration;

        // Find the remaining uncommitted time of the current frame


        return Result::NotYetImplemented;
    }

    Result Score::UpdateHarmonyGraph(HarmonicTransition& transition)
    {
        // Based on the transition duration, try to find an exit node,
        // or the dominant chord of the target scale
        // or a matching resolution with the target entry edge of the next graph
        return Result::NotYetImplemented;
    }
}
