#include "score.h"
#include "graph.h"
#include "edge.h"
#include "node.h"
#include "random.h"
#include "scale.h"

namespace Dryad
{
    Score::Score()
        : m_Ledger(0.0f, DefaultTempo, &MajorScale)
        , m_HarmonyFrames(m_DefaultHarmonicFramesCount)
    {
    }

    void Score::Reset(Time startTime, Tempo startTempo, const Scale* startScale)
    {
        m_Ledger = ScoreLedger(startTime, startTempo, startScale);
        //_events.Clean(); TODO: Clean ledger events
        m_HarmonyFrames.Clear();
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
        return m_HarmonyStrategy.ApplyTransition(*this, transition);
    }

    Result Score::UpdateMotifs(Map<const Motif*, Int32>& motifVariations)
    {
        Vector<const Motif*> motifsToRemove;
        for(const auto& [motif, variation] : motifVariations)
        {
            Int32 motifLevel = static_cast<Int32>(m_MotifLevels[motif]) + variation;
            if(motifLevel <= 0)
            {
                motifsToRemove.PushBack(motif);
            }
            else
            {
                m_MotifLevels[motif] = static_cast<UInt32>(motifLevel);
            }
        }
        for(const Motif* motif : motifsToRemove)
        {
            m_MotifLevels.Remove(motif);
        }

        return Result::Success;
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

    Result Score::UpdateNotes()
    {
        // Here, we will align the uncommitted notes of the score to the active harmomy
        // frames and motif.

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
        return m_HarmonyFrames.Front();
    }

    const HarmonyFrame& Score::CurrentHarmonyFrame() const
    {
        return m_HarmonyFrames.Front();
    }

    List<HarmonyFrame>& Score::GetHarmonyFrames()
    {
        return m_HarmonyFrames;
    }

    const List<HarmonyFrame>& Score::GetHarmonyFrames() const
    {
        return m_HarmonyFrames;
    }

    Tempo Score::CurrentTempo() const
    {
        if(m_HarmonyFrames.Empty())
        {
            return m_Ledger.startTempo;
        }
        return CurrentHarmonyFrame().tempo;
    }

    const Scale* Score::CurrentScale() const
    {
        if(m_HarmonyFrames.Empty())
        {
            return m_Ledger.startScale;
        }
        return CurrentHarmonyFrame().scale;
    }

    const Node* Score::CurrentNode()
    {
        if(m_HarmonyFrames.Empty())
        {
            return nullptr;
        }
        return CurrentHarmonyFrame().node;
    }

    ScoreTime Score::CurrentTime() const
    {
        return m_Ledger.committedDuration;
    }

    ScoreTime Score::TimeRemainingToCurrentHarmonyFrame() const
    {
        if(m_HarmonyFrames.Empty())
        {
            return 0;
        }
        return m_Ledger.committedDuration - CurrentHarmonyFrame().frameStart;
    }

    ScoreTime Score::CurrentHarmonyFrameEndTime() const
    {
        if(m_HarmonyFrames.Empty())
        {
            return 0;
        }
        return CurrentHarmonyFrame().EndTime();
    }

    Result Score::GenerateFrames(ScoreTime durationToAppend)
    {
        if(m_HarmonyFrames.Empty())
        {
            return Result::NotYetImplemented;
        }
        const Node* node = m_HarmonyFrames.Back().node;
        if(node == nullptr || !node->IsValid())
        {
            return Result::InvalidNode;
        }
        ScoreTime scoreEnd = m_HarmonyFrames.Back().EndTime();
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
                CurrentScale(),
                nextNode,
                nextNode->graph
            );
            m_HarmonyFrames.PushBack(newFrame);
            scoreEnd = newFrame.EndTime();
        }
        return Result::Success;
    }

    ScoreTime Score::GeneratedEndTime() const
    {
        if(m_HarmonyFrames.Empty())
        {
            return 0;
        }
        return m_HarmonyFrames.Back().EndTime();
    }

    Result Score::GenerateFramesUntil(ScoreTime targetScoreTime)
    {
        ScoreTime currentScoreEnd = GeneratedEndTime();
        if(targetScoreTime > currentScoreEnd)
        {
            return GenerateFrames(targetScoreTime - currentScoreEnd);
        }
        return Result::Success;
    }
}
