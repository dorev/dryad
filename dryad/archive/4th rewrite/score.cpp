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
            if (motif == nullptr)
            {
                LOG_ERROR("Motif variations vector contains a `nullptr` motif.");
                continue;
            }

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
                        ScoreFrame* scoreFrame = firstStagedScoreFrame;
                        while (scoreFrame != nullptr)
                        {
                            // Find notes corresponding to that motif in the frame
                            for (ScoreEvent* scoreEvent : scoreFrame->scoreEvents)
                            {
                                if (scoreEvent != nullptr
                                    && scoreEvent->IsPlayNote()
                                    && scoreEvent->GetNoteData().GetMotif() == motif)
                                {
                                    const ScoreNoteEvent& note = scoreEvent->GetNoteData();
                                    const MotifInstance* motifInstance = note.motifInstance;


                                    scoreFrame->scoreEvents.Remove(scoreEvent);
                                }
                            }
                            scoreFrame = scoreFrame->next;
                        }
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

            // Find score frame to associate with the motif instance beginning
            bool foundScoreFrameMatchingTime = false;
            while (scoreFrame != nullptr && scoreFrame->startTime <= motifStartTime)
            {
                if (motifStartTime == scoreFrame->startTime)
                {
                    newMotifInstance->SetStartFrame(scoreFrame);
                    foundScoreFrameMatchingTime = true;
                    break;
                }
            }
            if (!foundScoreFrameMatchingTime)
            {
                scoreFrame = GetOrCreateScoreFrame(motifStartTime);
                if (scoreFrame == nullptr)
                {
                    return Result::OperationFailed;
                }
                newMotifInstance->SetStartFrame(scoreFrame);
            }

            // Adjust note height to match the current notes
            NoteValue motifReferenceNote = GetLatestOctaveRoot();
            m_MotifInstances[motif].PushBack(newMotifInstance);

            // Naively align the motif on the root, the voicing/harmonization will be done later
            Result result = newMotifInstance->UpdateNotes(motifReferenceNote);
            RETURN_RESULT_ON_FAILURE(result);

            for (Note& note : newMotifInstance->GetNotes())
            {
                scoreFrame = GetOrCreateScoreFrame(note.startTime);
                if (scoreFrame != nullptr)
                {
                    scoreFrame->AddNote(note);
                    note.scoreFrame = scoreFrame;
                }
                else
                {
                    LOG_ERROR("Unable to insert motif note in score frame.");
                    return Result::OperationFailed;
                }
            }
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
            Result result = InsertHarmonyFrame(newFrame);
            RETURN_RESULT_ON_FAILURE(result);
            scoreEnd = newFrame->EndTime();
        }
        return Result::Success;
    }

    void Score::Reset(RealTime startTime, Tempo startTempo, const Scale* startScale)
    {
        m_StartTime = startTime;
        m_StartTempo = startTempo;
        m_StartScale = startScale;

        // Delete all the committed score frames
        ScoreFrame* scoreFrameToDelete = GetFirstCommittedFrame();
        ScoreFrame* nextScoreFrameToDelete = nullptr;
        while(scoreFrameToDelete != nullptr)
        {
            nextScoreFrameToDelete = scoreFrameToDelete->next;
            SafeDelete(scoreFrameToDelete);
            scoreFrameToDelete = nextScoreFrameToDelete;
        }
        m_LastCommittedScoreFrame = nullptr;

        // Delete all the staged score frames
        scoreFrameToDelete = GetFirstStagedFrame();
        while(scoreFrameToDelete != nullptr)
        {
            nextScoreFrameToDelete = scoreFrameToDelete->next;
            SafeDelete(scoreFrameToDelete);
            scoreFrameToDelete = nextScoreFrameToDelete;
        }
        m_LastStagedScoreFrame = nullptr;
    }

    HarmonyFrame* Score::CurrentHarmonyFrame()
    {
        return HarmonyFrameBackwardSearch(GetLastCommittedFrame());
    }

    const HarmonyFrame* Score::CurrentHarmonyFrame() const
    {
        return HarmonyFrameBackwardSearch(GetLastCommittedFrame());
    }

    HarmonyFrame* Score::LastHarmonyFrame()
    {
        return HarmonyFrameBackwardSearch(GetLastStagedFrame());
    }

    const HarmonyFrame* Score::LastHarmonyFrame() const
    {
        return HarmonyFrameBackwardSearch(GetLastStagedFrame());
    }

    HarmonyFrame* Score::HarmonyFrameBackwardSearch(const ScoreFrame* scoreFrame)
    {
        HarmonyFrame* harmonyFrame = nullptr;
        while (scoreFrame != nullptr)
        {
            for (ScoreEvent* event : scoreFrame->scoreEvents)
            {
                if (event != nullptr && event->type == ScoreEventType::HarmonyFrameChange)
                {
                    return event->GetHarmonyFrameData();
                }
            }
            scoreFrame = scoreFrame->prev;
        }
        return harmonyFrame;
    }

    const HarmonyFrame* Score::HarmonyFrameBackwardSearch(const ScoreFrame* scoreFrame) const
    {
        const HarmonyFrame* harmonyFrame = nullptr;
        while (scoreFrame != nullptr)
        {
            for (const ScoreEvent* event : scoreFrame->scoreEvents)
            {
                if (event != nullptr && event->type == ScoreEventType::HarmonyFrameChange)
                {
                    return event->GetHarmonyFrameData();
                }
            }
            scoreFrame = scoreFrame->prev;
        }
        return harmonyFrame;
    }

    ScoreFrame*& Score::GetFirstStagedFrame()
    {
        return m_FirstStagedScoreFrame;
    }

    ScoreFrame*& Score::GetFirstCommittedFrame()
    {
        return m_FirstCommittedScoreFrame;
    }

    ScoreFrame*& Score::GetLastCommittedFrame()
    {
        return m_LastCommittedScoreFrame;
    }

    ScoreFrame*& Score::GetLastStagedFrame()
    {
        return m_LastStagedScoreFrame;
    }

    const ScoreFrame* Score::GetLastStagedFrame() const
    {
        return m_LastStagedScoreFrame;
    }

    const ScoreFrame* Score::GetLastCommittedFrame() const
    {
        return m_LastCommittedScoreFrame;
    }

    Tempo Score::CurrentTempo() const
    {
        const HarmonyFrame* harmonyFrame = CurrentHarmonyFrame();
        if (harmonyFrame == nullptr)
        {
            return DefaultTempo;
        }
        return harmonyFrame->tempo;
    }

    const Scale* Score::CurrentScale() const
    {
        const HarmonyFrame* harmonyFrame = CurrentHarmonyFrame();
        if (harmonyFrame == nullptr)
        {
            return &MajorScale;
        }
        return harmonyFrame->scale;
    }

    const Node* Score::CurrentNode()
    {
        const HarmonyFrame* harmonyFrame = CurrentHarmonyFrame();
        if (harmonyFrame == nullptr)
        {
            return nullptr;
        }
        return harmonyFrame->node;
    }

    ScoreTime Score::CurrentTime() const
    {
        const ScoreFrame* scoreFrame = GetLastCommittedFrame();
        return scoreFrame == nullptr ? 0 : scoreFrame->startTime;
    }

    ScoreTime Score::TimeRemainingToCurrentHarmonyFrame() const
    {
        const HarmonyFrame* harmonyFrame = CurrentHarmonyFrame();
        if (harmonyFrame == nullptr)
        {
            return 0;
        }
        return harmonyFrame->EndTime() - CurrentTime();
    }

    NoteValue Score::GetLastCommittedNoteValue() const
    {
        // Walk back through the frames from the last one and return the first note event found
        const ScoreFrame* scoreFrame = GetLastCommittedFrame();
        while (scoreFrame != nullptr)
        {
            for (const ScoreEvent* event : scoreFrame->scoreEvents)
            {
                if (event->type == ScoreEventType::PlayNote)
                {
                    return event->GetNoteData().value;
                }
            }
            scoreFrame = scoreFrame->prev;
        }
        return MiddleC;
    }

    NoteValue Score::GetLatestOctaveRoot() const
    {
        const HarmonyFrame* harmonyFrame = CurrentHarmonyFrame();
        if (harmonyFrame == nullptr)
        {
            const Scale* scale = CurrentScale();
            return MiddleC + scale->root;
        }
        else
        {
            NoteValue root = harmonyFrame->scale->root;
            NoteValue reference = GetLastCommittedNoteValue() % Notes[C][1];
            return reference + root;
        }
    }

    ScoreTime Score::GeneratedEndTime() const
    {
        const HarmonyFrame* harmonyFrame = CurrentHarmonyFrame();
        if (harmonyFrame == nullptr)
        {
            return 0;
        }
        return harmonyFrame->EndTime();
    }

    Result Score::GenerateHarmonyFramesUntil(ScoreTime targetScoreTime)
    {
        ScoreTime currentScoreEnd = GeneratedEndTime();
        if (targetScoreTime > currentScoreEnd)
        {
            return GenerateHarmonyFrames(targetScoreTime - currentScoreEnd);
        }
        return Result::Success;
    }

    Result Score::ClearHarmonyFramesAfter(HarmonyFrame* harmonyFrame)
    {
        if (harmonyFrame == nullptr)
        {
            return Result::UselessOperation;
        }
        ScoreFrame* scoreFrame = harmonyFrame->scoreFrame;
        if (scoreFrame == nullptr)
        {
            return Result::InvalidHarmonyFrame;
        }
        harmonyFrame->next = nullptr;

        // Find all the following score frames with harmony frame events and delete
        // the harmony event
        scoreFrame = scoreFrame->next;
        while (scoreFrame != nullptr)
        {
            harmonyFrame = scoreFrame->GetHarmonyFrame();
            if (harmonyFrame != nullptr)
            {
                scoreFrame->DeleteHarmonyFrame();
            }
            scoreFrame = scoreFrame->next;
        }
        return Result::Success;
    }

    Result Score::ClearHarmonyFramesAfter(ScoreTime time)
    {
        // We start at the latest committed harmony frame because we cannot clear frames from the past
        HarmonyFrame* harmonyFrame = CurrentHarmonyFrame();
        if (harmonyFrame == nullptr)
        {
            LOG_WARN("Cannot clear harmony frames after score time %d because there are no harmony frames", time);
            return Result::UselessOperation;
        }

        // Find the latest frame possible
        while (harmonyFrame->next != nullptr && harmonyFrame->next->startTime < time)
        {
            harmonyFrame = harmonyFrame->next;
        }
        return ClearHarmonyFramesAfter(harmonyFrame);
    }

    Result Score::InsertHarmonyFrame(HarmonyFrame* harmonyFrame)
    {

        if (harmonyFrame == nullptr)
        {
            LOG_WARN("Cannot insert null harmony frame");
            return Result::UselessOperation;
        }

        if (CurrentTime() >= harmonyFrame->startTime)
        {
            LOG_WARN("Cannot insert harmony frame into a time slot that's already committed.");
            return Result::OperationFailed;
        }

        // First, check if we can insert it into the staged frames.
        ScoreFrame* scoreFrame = GetFirstStagedFrame();
        while (scoreFrame != nullptr)
        {
            if (scoreFrame->startTime == harmonyFrame->startTime)
            {
                if (scoreFrame->HasHarmonyChanges())
                {
                    LOG_WARN("A ScoreFrame with matching start time already has a HarmonyFrame.");
                    return Result::OperationFailed;
                }

                scoreFrame->AddHarmonyFrame(harmonyFrame);
                return Result::Success;
            }
            else if (scoreFrame->startTime > harmonyFrame->startTime)
            {
                // Insert the HarmonyFrame before the current ScoreFrame in a new ScoreFrame.
                
                ScoreFrame* newFrame = new ScoreFrame(harmonyFrame->startTime);
                newFrame->AddHarmonyFrame(harmonyFrame);
                newFrame->InsertBefore(scoreFrame);
                return Result::Success;
            }
        }

        // If we reach here, the HarmonyFrame's startTime is after all the staged ScoreFrames.
        // Add it to the end.
        ScoreFrame* newFrame = new ScoreFrame(harmonyFrame->startTime);
        newFrame->AddHarmonyFrame(harmonyFrame);
        scoreFrame = GetLastStagedFrame();
        if (scoreFrame = nullptr)
        {
            GetLastStagedFrame() = newFrame;
            GetFirstStagedFrame() = newFrame;
        }
        else
        {
            newFrame->InsertAfter(scoreFrame);
        }

        return Result::Success;
    }

    ScoreFrame* Score::GetOrCreateScoreFrame(ScoreTime startTime)
    {
        ScoreFrame* scoreFrame = GetLastCommittedFrame();
        if (scoreFrame != nullptr && scoreFrame->startTime <= startTime)
        {
            LOG_ERROR("Unable to insert ScoreFrame within committed frames.");
            return nullptr;
        }
        scoreFrame = GetFirstStagedFrame();
        while (scoreFrame != nullptr)
        {
            if (scoreFrame->startTime == startTime)
            {
                return scoreFrame;
            }
            else if(scoreFrame->startTime > startTime)
            {
                ScoreFrame* newScoreFrame = new ScoreFrame(startTime);
                newScoreFrame->InsertBefore(scoreFrame);
                return newScoreFrame;
            }
            scoreFrame = scoreFrame->next;
        }

        // Reaching this point, the new frame must be the last
        scoreFrame = GetLastStagedFrame();
        ScoreFrame* newScoreFrame = new ScoreFrame(startTime);
        if (scoreFrame == nullptr)
        {
            m_FirstStagedScoreFrame = m_LastStagedScoreFrame = newScoreFrame;
        }
        else
        {
            newScoreFrame->InsertAfter(scoreFrame);
        }
        return newScoreFrame;
    }
}
