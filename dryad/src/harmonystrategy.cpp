#include "types.h"
#include "harmonytransition.h"
#include "harmonystrategy.h"
#include "score.h"
#include "scale.h"

namespace Dryad
{
    Result HarmonyStrategy::ApplyTransition(Score& score, HarmonyTransition& transition)
    {
        Node* node = transition.GetEntryNode();
        const Scale* scale = transition.scale;
        const Graph* graph = transition.graph;

        // A node can be nullptr (in the case where we only change the scale)
        if (node != nullptr && node->IsNotValid())
        {
            return Result::InvalidNode;
        }
        else if (node == score.CurrentNode())
        {
            // If we're are pointing to the same node, we need at least a scale change
            if (scale == nullptr || score.CurrentScale() == scale)
            {
                return Result::UselessOperation;
            }
            else
            {
                return ChangeScale(score, transition);
            }
        }
        else
        {
            return ChangeGraph(score, transition);
        }
    }

    Result HarmonyStrategy::FirstFrame(Score& score, HarmonyTransition& transition)
    {
        Node* node = transition.GetEntryNode();
        if (node == nullptr)
        {
            return Result::NodeNotFound;
        }
        if (node->IsNotValid())
        {
            return Result::InvalidNode;
        }

        // Prepare the first frame
        HarmonyFrame* frame = new HarmonyFrame();
        const Scale* scale = transition.scale;

        // The frame scale has to be specified, or the default scale of the score will be used
        frame->scale = (scale != nullptr) ? scale : score.CurrentScale();

        // Complete frame setup and add queue it
        frame->startTime = 0;
        frame->duration = node->duration;
        frame->graph = node->graph;
        frame->tempo = score.CurrentTempo();
        score.InsertHarmonyFrame(frame);
        return Result::Success;
    }

    Result HarmonyStrategy::ChangeScale(Score& score, HarmonyTransition& transition)
    {
        ScoreTime currentTime = score.CurrentTime();
        HarmonyFrame* harmonyFrame = score.CurrentHarmonyFrame();

        // Handle the case where a transition is requested on an empty score
        if (harmonyFrame == nullptr)
        {
            return FirstFrame(score, transition);
        }

        // If the transition has to happen within the frame, make it happen on the next beat
        // by splitting the current HarmonyFrame at that time and change the scale on that latter frame
        if (transition.maxDuration < score.TimeRemainingToCurrentHarmonyFrame())
        {
            ScoreTime nextBeat = NearestBeatAfter(Quarter, currentTime);
            HarmonyFrame* nextHarmonyFrame = new HarmonyFrame(*harmonyFrame);
            Result result = harmonyFrame->SplitFrame(nextBeat, *nextHarmonyFrame);
            RETURN_RESULT_ON_FAILURE(result);

            // Remove all the frames that are after the split
            nextHarmonyFrame->scale = transition.scale;
            result = score.InsertHarmonyFrame(nextHarmonyFrame);
            RETURN_RESULT_ON_FAILURE(result);
            harmonyFrame = nextHarmonyFrame;
        }

        // Update the following harmony frame scales
        harmonyFrame = harmonyFrame->next;
        while (harmonyFrame != nullptr)
        {
            // TODO: this will have to take edge modulation into account!
            // something like harmonyFrame.ApplyScaleDiff(sourceScale, destinationScale);
            // or maybe this is something that could be validated at commit??
            harmonyFrame->scale = transition.scale;
            harmonyFrame = harmonyFrame->next;
        }
        return Result::Success;
    }

    Result HarmonyStrategy::ChangeGraph(Score& score, HarmonyTransition& transition)
    {
        const Graph* graph = transition.graph;
        const Node* node = transition.GetEntryNode();
        const Scale* scale = transition.scale;
        HarmonyFrame* harmonyFrame = score.CurrentHarmonyFrame();

        // Handle the case where a transition is requested on an empty score
        if (harmonyFrame == nullptr)
        {
            return FirstFrame(score, transition);
        }

        // Sanity checks
        if (graph == nullptr || graph->nodes.Empty())
        {
            return Result::InvalidGraph;
        }
        if (graph->entryEdges.Empty())
        {
            return Result::InvalidEdge;
        }
        if (scale == nullptr)
        {
             return Result::InvalidScale;
        }

        // Trim any frame beyond the transition deadline
        // The soonest a transition can happen is the next frame
        ScoreTime deadline = score.CurrentTime() + transition.maxDuration;
        Result result = score.GenerateHarmonyFramesUntil(deadline);
        RETURN_RESULT_ON_FAILURE(result);
        score.ClearHarmonyFramesAfter(deadline);

        // Identify if there is an exit frame in the remaining frames
        // If no exit frame is found, the transition will simply occur on the latest frame possible
        harmonyFrame = score.LastHarmonyFrame();
        while (harmonyFrame != nullptr)
        {
            const Node* frameNode = harmonyFrame->node;
            if (frameNode != nullptr && frameNode->graphExit)
            {
                score.ClearHarmonyFramesAfter(harmonyFrame);
                break;
            };
            harmonyFrame = harmonyFrame->prev;
        }
        if (harmonyFrame == nullptr)
        {
            return Result::InvalidHarmonyFrame;
        }

        // NOTE: eventually, we could try to find a better match between the last/exit node
        //       and the entry node of the next graph

        // If no entry is specified, use a random entry node
        if (node == nullptr)
        {
            const Edge* randomEntryEdge = nullptr;
            result = RandomFrom(graph->entryEdges, randomEntryEdge);
            RETURN_RESULT_ON_FAILURE(result);
            node = randomEntryEdge->destination;
        }

        HarmonyFrame* nextFrame = new HarmonyFrame
        (
            score.CurrentTempo(),
            harmonyFrame->EndTime(),
            node->duration,
            transition.scale,
            node,
            graph
        );
        score.InsertHarmonyFrame(nextFrame);
        return Result::Success;
    }
}
