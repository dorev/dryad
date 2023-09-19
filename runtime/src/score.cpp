#include "score.h"
#include "graph.h"
#include "edge.h"
#include "node.h"
#include "random.h"
#include "scale.h"
#include "motif.h"
#include "motifinstance.h"

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
        if (graph != nullptr)
        {
            // Validate that the provided edge belongs to the graph...
            if (entryEdge != nullptr)
            {
                if (!graph->HasEntryEdge(entryEdge))
                {
                    return Result::EdgeNotFound;
                }
            }
            // ...or select randomly an entry edge
            else
            {
                result = RandomFrom(graph->entryEdges, entryEdge);
                if (result != Result::Success)
                {
                    return result;
                }
                ResetResult(result);
            }
        }
        else if (entryEdge == nullptr)
        {
            return Result::EdgeNotFound;
        }

        // Now we validate the entry node
        Node* node = entryEdge->destination;
        if (node == nullptr)
        {
            return Result::NodeNotFound;
        }
        if (node->graph == nullptr)
        {
            return Result::GraphNotFound;
        }

        // Validate that if a graph was provided, it's correctly associated to the node
        if (graph != nullptr && node->graph != graph)
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
            if (motifLevel <= 0)
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

        // !! For the first version, let's only support instantaneous tempo changes

        return Result::NotYetImplemented;
    }

    Result Score::UpdateNotes(Map<const Motif*, Int32>& motifsVariations, const HarmonyTransition& harmonyTransition)
    {
        const HarmonyFrame& harmonyFrame = CurrentHarmonyFrame();
        ScoreFrame* firstUncommittedScoreFrame = m_Ledger.GetFirstUncommittedFrame();

        // Add motifs notes first, so all the notes can already be then when reharmonizing
        for (const auto& [motif, variation] : motifsVariations)
        {
            // If the motif is not already in the score, add it
            if (!m_MotifLevels.Contains(motif))
            {
                m_MotifLevels[motif] = 0;
            }
            int newLevel = m_MotifLevels[motif] += variation;

            // Adding a motif
            if (variation > 0)
            {
                // Find the best time to start the motif
                ScoreTime motifStartTime = 0;
                if (motif->alignRhythmToNode)
                {
                    // If the ledger and harmony frames are aligned, start the motif at the beginning of the frame
                    if (firstUncommittedScoreFrame->startTime == harmonyFrame.startTime)
                    {
                        motifStartTime = firstUncommittedScoreFrame->startTime;
                    }
                    else
                    {
                        // If the time is not already aligned with the motif, find the next aligned time
                        motifStartTime = NearestBeatAfter(motif->rhythmicAlignment, harmonyFrame.startTime);
                        while (motifStartTime < firstUncommittedScoreFrame->startTime)
                        {
                            motifStartTime += motif->rhythmicAlignment;
                        }

                        // If the first aligned time is beyond the current frame, align it with the next frame
                        if (motifStartTime > harmonyFrame.EndTime())
                        {
                            motifStartTime = harmonyFrame.EndTime();
                        }
                    }
                }
                else
                {
                    motifStartTime = NearestBeatAfter(motif->rhythmicAlignment, firstUncommittedScoreFrame->startTime);
                }

                MotifInstance* newMotifInstance = new MotifInstance(motif, motifStartTime);
                NoteValue motifReferenceNote = GetLatestOctaveRoot();
                m_MotifInstances[motif].PushBack(newMotifInstance);

                // Naively align the motif on the root, the voicing/harmonization will be done later
                newMotifInstance->UpdateNotes(motifReferenceNote);
            }
            // Removing a motif
            else
            {
                // Find all the instances of this motif
                List<MotifInstance*> motifInstances = m_MotifInstances[motif];
                if (motifInstances.Size() == 0)
                {
                    // Reaching this area is suspicious... at this point if we identified this
                    // motif it should contain instances...
                    // Erase from the instances map and move on to the next motif
                    m_MotifInstances.Remove(motif);
                    continue;
                }
                if (newLevel == 0)
                {
                    // Remove any uncommitted notes of the motif
                    if (motif->canBeTruncated)
                    {
                        // From the first uncommitted frame to the end of the score,
                        // remove any note event associated to the current motif
                        ScoreFrame* scoreFrame = firstUncommittedScoreFrame;
                        while (scoreFrame != nullptr)
                        {
                            for (ScoreEvent* scoreEvent : scoreFrame->events)
                            {
                                if (scoreEvent != nullptr
                                    && scoreEvent->type == ScoreEventType::PlayNote
                                    && scoreEvent->GetNoteEvent().motif == motif)
                                {
                                    scoreFrame->events.Remove(scoreEvent);
                                }
                            }
                            scoreFrame = scoreFrame->next;
                        }
                    }
                    else
                    // Remove any next iteration of the motif
                    {
                        // Ok now... how do we find the notes of specific motif instances now...
                        // Check all the 
                    }

                    m_MotifInstances.Remove(motif);
                }
                else
                {

                }
                return Result::NotYetImplemented;
            }

            // OOOUPS!! Seems to be a problem in there... we're adding the notes to the motif instance
            // but not to the score ledger! That suuuuucks! It would be much better if the motif instances could
            // store the score frames they're starting on, that would be much easier for look-ups
        }

        // When processing to reharmonization...
            // According to the current notes position, establish which motive should be the lowest note of the frame chord
            // 'Hints' could be associated to motifs during reharmonization to indicate how to attempt to place them
        // A second pass should probably be done to smoothen the voicings

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
        if (m_HarmonyFrames.Empty())
        {
            return m_Ledger.GetStartTempo();
        }
        return CurrentHarmonyFrame().tempo;
    }

    const Scale* Score::CurrentScale() const
    {
        if (m_HarmonyFrames.Empty())
        {
            // TODO: return the default scale?
            return m_Ledger.GetStartScale();
        }
        return CurrentHarmonyFrame().scale;
    }

    const Node* Score::CurrentNode()
    {
        if (m_HarmonyFrames.Empty())
        {
            return nullptr;
        }
        return CurrentHarmonyFrame().node;
    }

    ScoreTime Score::CurrentTime() const
    {
        return m_Ledger.GetCommittedScoreDuration();
    }

    ScoreTime Score::TimeRemainingToCurrentHarmonyFrame() const
    {
        if (m_HarmonyFrames.Empty())
        {
            return 0;
        }
        return CurrentHarmonyFrameEndTime() - CurrentTime();
    }

    ScoreTime Score::CurrentHarmonyFrameEndTime() const
    {
        if (m_HarmonyFrames.Empty())
        {
            return 0;
        }
        return CurrentHarmonyFrame().EndTime();
    }

    NoteValue Score::GetLatestOctaveRoot() const
    {
        if (m_HarmonyFrames.Empty())
        {
            const Scale* scale = CurrentScale();
            return MiddleC + scale->root;
        }
        else
        {
            NoteValue root = m_HarmonyFrames.Back().scale->root;
            NoteValue reference = m_Ledger.GetLastCommittedNoteValue() % Notes[C][1];
            return reference + root;
        }
    }

    ScoreTime Score::GeneratedEndTime() const
    {
        if (m_HarmonyFrames.Empty())
        {
            return 0;
        }
        return m_HarmonyFrames.Back().EndTime();
    }

    Result Score::GenerateFramesUntil(ScoreTime targetScoreTime)
    {
        ScoreTime currentScoreEnd = GeneratedEndTime();
        if (targetScoreTime > currentScoreEnd)
        {
            return GenerateFrames(targetScoreTime - currentScoreEnd);
        }
        return Result::Success;
    }

    Result Score::GenerateFrames(ScoreTime durationToAppend)
    {
        if (m_HarmonyFrames.Empty())
        {
            return Result::NotYetImplemented;
        }
        const Node* node = m_HarmonyFrames.Back().node;
        if (node == nullptr || !node->IsValid())
        {
            return Result::InvalidNode;
        }
        ScoreTime scoreEnd = m_HarmonyFrames.Back().EndTime();
        ScoreTime scoreTarget = scoreEnd + durationToAppend;
        while(scoreEnd < scoreTarget)
        {
            Node* nextNode = node->GetNext();
            if (nextNode == nullptr || !nextNode->IsValid())
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
}
