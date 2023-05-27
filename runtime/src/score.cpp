#include "score.h"
#include "graph.h"
#include "edge.h"
#include "node.h"
#include "random.h"

namespace Dryad
{
    Score::Score()
        : _info(0.0f)
        , _harmonyFrames(DefaultHarmonicFramesCount)
        , _events()
    {
    }

    void Score::Reset(Time startTime)
    {
        _info = ScoreInfo(startTime);
        _harmonyFrames.Reset(DefaultHarmonicFramesCount);
        _events.Clean();
    }

    Result Score::UpdateHarmony(HarmonyTransition& transition)
    {
        Result result = Result:: EmptyResult;
        Graph* graph = transition.graph;
        Edge* entryEdge = transition.entryEdge;

        // No frames yet
        if(_harmonyFrames.Empty())
        {
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

            // Initialize with default frame
            HarmonyFrame frame;
            if(transition.scale != nullptr)
            {
                frame.scale = transition.scale;
            }

            // Complete frame setup and add queue it
            frame.duration = node->duration;
            frame.frameStart = ScoreTime(1, 4, 0); // time should be updated at commit
            frame.graph = node->graph;
            frame.timeSignature = node->graph->timeSignature;
            frame.tempo; // tempo should be set at commit
            return Result::Success;
        }

        // Based on the transition time limit...
        //  * check if we can move toward an exit node
        //  * otherwise proceed to the next graph a the closest node finish point
        // ... cute strategies will follow later!!

        // In the case of a scale change, try to follow common chords or to switch
        // after the target scale dominant

        // Skip edge modulation when changing scale or graph
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
        return Result::NotYetImplemented;
    }
}
