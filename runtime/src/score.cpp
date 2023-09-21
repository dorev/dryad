#include "score.h"
#include "graph.h"
#include "edge.h"
#include "node.h"
#include "random.h"
#include "scale.h"
#include "motif.h"
#include "motifinstance.h"
#include "scoreevent.h"

namespace Dryad
{
    Score::Score()
        : m_StartTime(0.0f)
        , m_StartTempo(DefaultTempo)
        , m_StartScale(&MajorScale)
    {
    }

    // Handles the motification to harmony frames
    Result Score::UpdateHarmony(HarmonyTransition& transition)
    {
        Result result = Result:: EmptyResult;
        const Graph* graph = transition.graph;
        const Edge* entryEdge = transition.entryEdge;

        // Validate the entry edge
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

    // Handles the concurrent count of identical motifs
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

    // Handles adding or removing note to the uncommitted score space based on motif changes
    // that occured since the last update
    Result Score::UpdateNotes(Map<const Motif*, Int32>& motifsVariations, const HarmonyTransition& harmonyTransition)
    {
        const HarmonyFrame* harmonyFrame = CurrentHarmonyFrame();
        ScoreFrame* firstStagedScoreFrame = GetFirstStagedFrame();

        // Add motifs notes first, so all the notes can already be then when reharmonizing
        for (const auto& [motif, variation] : motifsVariations)
        {
            // If the motif is not already in the score, add it
            if (!m_MotifLevels.Contains(motif))
            {
                m_MotifLevels[motif] = 0;
            }
            Int32 newLevelSigned = m_MotifLevels[motif] + variation;
            UInt32 newLevel = newLevelSigned < 0 ? 0 : static_cast<UInt32>(newLevelSigned);
            m_MotifLevels[motif] = newLevel;

            // Adding a motif
            if (variation > 0)
            {
                return AddMotif(motif, variation, firstStagedScoreFrame);
            }
            // Removing a motif
            else
            {
                // Find all the instances of this motif
                List<MotifInstance*> motifInstances = m_MotifInstances[motif];
                if (motifInstances.Size() == 0)
                {
                    // Reaching this area is suspicious, at this point if we identified this
                    // motif it should contain instances...
                    // Erase from the instances map and move on to the next motif
                    m_MotifInstances.Remove(motif);
                    continue;
                }
                if (newLevel == 0)
                {
                    // Remove any staged notes of the motif
                    if (motif->canBeTruncated)
                    {
                        // HEY COULD I USE THE LISTED INSTANCES HERE??


                        // From the first staged frame to the end of the score,
                        // remove all note event associated to the current motif
                        ScoreFrame* scoreFrame = firstStagedScoreFrame;
                        while (scoreFrame != nullptr)
                        {
                            for (ScoreEvent* scoreEvent : scoreFrame->scoreEvents)
                            {
                                if (scoreEvent != nullptr 
                                    && scoreEvent->IsPlayNote()
                                    && scoreEvent->GetNoteData().GetMotif() == motif)
                                {
                                    scoreFrame->scoreEvents.Remove(scoreEvent);
                                }
                            }
                            scoreFrame = scoreFrame->next;
                        }
                    }
                    // Remove any next iteration of the motif
                    else
                    {
                        // Ok now... how do we find the notes of specific motif instances now...
                        // On all the staged frames
                            // Check the presence of motif instances
                            // Find if the start frame of this motif is already committed
                            // If it is, go to the next note
                            // If it isn't remove all the notes associated to that motif instance
                    }

                    m_MotifInstances.Remove(motif);
                }
                else
                {
                    // At first, try to remove staged motif instances only
                    // Start by listing the instances?


                    // Remove any staged notes of the motif
                    if (motif->canBeTruncated)
                    {

                    }
                    // Remove any next iteration of the motif
                    else
                    {

                    }
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

    Result Score::AddMotif(const Motif* motif, UInt32 count, ScoreFrame* scoreFrame)
    {
        if (motif == nullptr)
        {
            return Result::InvalidMotif;
        }
        if (scoreFrame == nullptr)
        {
            return Result::InvalidScoreFrame;
        }
        HarmonyFrame* harmonyFrame = scoreFrame->GetHarmonyFrame();
        if (harmonyFrame == nullptr)
        {
            return Result::InvalidHarmonyFrame;
        }

        // NOTE: It could be interesting to be able to inject a motif insertion strategy here!
        while (count-- > 0)
        {
            // Find the best time to start the motif
            ScoreTime motifStartTime = 0;
            if (motif->alignRhythmToNode)
            {
                // If the score and harmony frames are aligned, start the motif at the beginning of the frame
                if (scoreFrame->startTime == harmonyFrame->startTime)
                {
                    motifStartTime = scoreFrame->startTime;
                }
                else
                {
                    // If the time is not already aligned with the motif, find the next aligned time
                    motifStartTime = NearestBeatAfter(motif->rhythmicAlignment, harmonyFrame->startTime);
                    while (motifStartTime < scoreFrame->startTime)
                    {
                        motifStartTime += motif->rhythmicAlignment;
                    }

                    // If the first aligned time is beyond the current frame, align it with the next frame
                    if (motifStartTime > harmonyFrame->EndTime())
                    {
                        motifStartTime = harmonyFrame->EndTime();
                    }
                }
            }
            else
            {
                motifStartTime = NearestBeatAfter(motif->rhythmicAlignment, scoreFrame->startTime);
            }

            MotifInstance* newMotifInstance = new MotifInstance(motif, motifStartTime);
            NoteValue motifReferenceNote = GetLatestOctaveRoot();
            m_MotifInstances[motif].PushBack(newMotifInstance);

            // Naively align the motif on the root, the voicing/harmonization will be done later
            Result result = newMotifInstance->UpdateNotes(motifReferenceNote);
            RETURN_RESULT_ON_FAILURE(result);
        }

        return Result::Success;
    }

    //Result Score::RemoveMotif()
    //{
    //}

    Result Score::Commit(RealTime deltaTime, Vector<ScoreEvent>& newCommittedEvents)
    {
        // TODO: complete before starting editor work

        // This should be pretty straight-forward, just about scanning from the current
        // score position, toggling the notes bools and updating the vector
        // If we reach the end of a harmonic frame, prepare the next one if necessary.

        // It was noted somewhere that this function should check that a node scale properly respects edge modulations
        // But maybe that's not the best spot for that... 
        return Result::NotYetImplemented;
    }

    Result Score::GenerateHarmonyFrames(ScoreTime durationToAppend)
    {
        HarmonyFrame* harmonyFrame = LastHarmonyFrame();
        if (harmonyFrame == nullptr)
        {
            return Result::NotYetImplemented;
        }
        const Node* node = harmonyFrame->node;
        if (node == nullptr || !node->IsValid())
        {
            return Result::InvalidNode;
        }
        ScoreTime scoreEnd = harmonyFrame->EndTime();
        ScoreTime scoreTarget = scoreEnd + durationToAppend;
        while(scoreEnd < scoreTarget)
        {
            Node* nextNode = node->GetNext();
            if (nextNode == nullptr || !nextNode->IsValid())
            {
                return Result::InvalidNode;
            }
            HarmonyFrame* newFrame = new HarmonyFrame
            (
                CurrentTempo(),
                scoreEnd,
                nextNode->duration,
                CurrentScale(),
                nextNode,
                nextNode->graph
            );
            InsertHarmonyFrame(newFrame);
            scoreEnd = newFrame->EndTime();
        }
        return Result::Success;
    }
}
