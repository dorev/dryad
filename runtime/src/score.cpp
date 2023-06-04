#include "score.h"
#include "graph.h"
#include "edge.h"
#include "node.h"
#include "random.h"
#include "scale.h"

namespace Dryad
{
    Score::Score()
        : _ledger(0.0f, DefaultTempo, &MajorScale)
        , _harmonyFrames(DefaultHarmonicFramesCount)
    {
    }

    void Score::Reset(Time startTime, Tempo startTempo, const Scale* startScale)
    {
        _ledger = ScoreLedger(startTime, startTempo, startScale);
        //_events.Clean(); TODO: Clean ledger events
        _harmonyFrames.Reset(DefaultHarmonicFramesCount);
    }

    Result Score::UpdateHarmony(HarmonyTransition& transition)
    {
        Result result = Result:: EmptyResult;
        const Graph* graph = transition.graph;
        const Edge* entryEdge = transition.entryEdge;

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
        return _harmonyStrategy.ApplyTransition(*this, transition);
    }

    Result Score::UpdateMotifs(Map<Motif*, Int32>& motifVariations)
    {
        // TODO: complete before starting editor work

        // This will essentially be about updating the motif levels of the
        // current harmonic frame!
        return Result::NotYetImplemented;
    }

    Result Score::UpdateTempo(TempoChange& tempoChange)
    {
        // NOTE: optional for editor work debut

        // This will lead to the insertion of a ScoreEvent and also to some modification
        // of the harmonic frames (depending on the duration of the tempo change...)
        // I'm thinking about pushing this for a farther development cycle but I feel it
        // would be better to consider it sooner than later
        return Result::NotYetImplemented;
    }

    Result Score::Commit(Time deltaTime, Vector<ScoreEvent>& newCommittedEvents)
    {
        // TODO: complete before starting editor work

        // This should be pretty straight-forward, just about scanning from the current
        // score position, toggling the notes bools and updating the vector
        // If we reach the end of a harmonic frame, prepare the next one if necessary.
        // Keep always t
        return Result::NotYetImplemented;
    }

    HarmonyFrame& Score::CurrentHarmonyFrame()
    {
        return _harmonyFrames.Front();
    }

    const HarmonyFrame& Score::CurrentHarmonyFrame() const
    {
        return _harmonyFrames.Front();
    }

    Deque<HarmonyFrame>& Score::GetHarmonyFrames()
    {
        return _harmonyFrames;
    }

    const Deque<HarmonyFrame>& Score::GetHarmonyFrames() const
    {
        return _harmonyFrames;
    }

    Tempo Score::CurrentTempo() const
    {
        if(_harmonyFrames.Empty())
        {
            return _ledger.startTempo;
        }
        return CurrentHarmonyFrame().tempo;
    }

    const Scale* Score::CurrentScale() const
    {
        if(_harmonyFrames.Empty())
        {
            return _ledger.startScale;
        }
        return CurrentHarmonyFrame().scale;
    }

    Node* Score::CurrentNode()
    {
        if(_harmonyFrames.Empty())
        {
            return nullptr;
        }
        return CurrentHarmonyFrame().node;
    }

    ScoreTime Score::CurrentTime() const
    {
        return _ledger.committedDuration;
    }

    ScoreTime Score::TimeRemainingToCurrentHarmonyFrame() const
    {
        if(_harmonyFrames.Empty())
        {
            return 0;
        }
        return _ledger.committedDuration - CurrentHarmonyFrame().frameStart;
    }

    ScoreTime Score::CurrentHarmonyFrameEndTime() const
    {
        if(_harmonyFrames.Empty())
        {
            return 0;
        }
        return CurrentHarmonyFrame().FrameEnd();
    }

    Result Score::GenerateFrames(ScoreTime durationToAppend)
    {
        if(_harmonyFrames.Empty())
        {
            return Result::NotYetImplemented;
        }
        Node* node = _harmonyFrames.Back().node;
        if(node == nullptr || !node->IsValid())
        {
            return Result::InvalidNode;
        }
        ScoreTime scoreEnd = _harmonyFrames.Back().FrameEnd();
        ScoreTime scoreTarget = scoreEnd + durationToAppend;
        while(scoreEnd < scoreTarget)
        {
            Node* nextNode = node->GetNext();
            if(nextNode == nullptr || !nextNode->IsValid())
            {
                return Result::InvalidNode;
            }
            HarmonyFrame newFrame
            (
                CurrentTempo(),
                scoreEnd,
                nextNode->duration,
                {},
                CurrentScale(),
                nextNode,
                nextNode->graph
            );
            _harmonyFrames.PushBack(newFrame);
            scoreEnd = newFrame.FrameEnd();
        }
        return Result::Success;
    }
}
