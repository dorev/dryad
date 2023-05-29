#include "score.h"
#include "graph.h"
#include "edge.h"
#include "node.h"
#include "random.h"

namespace Dryad
{
    Score::Score()
        : _ledger(0.0f, DefaultTempo)
        , _harmonyFrames(DefaultHarmonicFramesCount)
    {
    }

    void Score::Reset(Time startTime, Tempo startTempo)
    {
        _ledger = ScoreLedger(startTime, startTempo);
        //_events.Clean(); TODO: Clean ledger events
        _harmonyFrames.Reset(DefaultHarmonicFramesCount);
    }

    Result Score::UpdateHarmony(HarmonyTransition& transition)
    {
        Result result = Result:: EmptyResult;
        Graph* graph = transition.graph;
        Edge* entryEdge = transition.entryEdge;

        // We must start by validating the entry edge
        if(graph != nullptr)
        {
            // Validate that the provided edge belongs to the graph...
            if(entryEdge != nullptr)
            {
                if(!graph->HasEntryEdge(entryEdge))
                {
                    return Result::EdgeNotFound;
                }
            }
            // ...or select randomly an entry edge
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
            return Result::EdgeNotFound;
        }

        // Now we validate the entry node
        Node* node = entryEdge->destination;
        if(node == nullptr)
        {
            return Result::NodeNotFound;
        }
        if(node->graph == nullptr)
        {
            return Result::GraphNotFound;
        }

        // Validate that if a graph was provided, it's correctly associated to the node
        if(graph != nullptr && node->graph != graph)
        {
            return Result::InvalidGraph;
        }

        // Now that the transition data has been validated, let the strategy do the rest
        return _harmonyStrategy.ApplyTransition(_harmonyFrames, transition);
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

    HarmonyFrame& Score::CurrentHarmonicFrame()
    {
        return _harmonyFrames.Front();
    }
    
    const HarmonyFrame& Score::CurrentHarmonicFrame() const
    {
        return _harmonyFrames.Front();
    }

    Tempo Score::CurrentTempo() const
    {
        if(_harmonyFrames.Empty())
        {
            return _ledger.startTempo;
        }
        return CurrentHarmonicFrame().tempo;
    }

    Scale* Score::CurrentScale() const
    {
        if(_harmonyFrames.Empty())
        {
            return nullptr;
        }
        return CurrentHarmonicFrame().scale;
    }

    Node* Score::CurrentNode()
    {
        if(_harmonyFrames.Empty())
        {
            return nullptr;
        }
        return CurrentHarmonicFrame().node;
    }
}
