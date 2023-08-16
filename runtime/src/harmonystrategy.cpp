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
        if (node != nullptr && !node->IsValid())
        {
            return Result::InvalidNode;
        }

        // Handle the case where a transition is requested on an empty score
        if (score.GetHarmonyFrames().Empty())
        {
            return FirstFrame(score, transition);
        }
        if (node == score.CurrentNode())
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
        if (!node->IsValid())
        {
            return Result::InvalidNode;
        }

        // Prepare the first frame
        HarmonyFrame frame;
        const Scale* scale = transition.scale;

        // The frame scale has to be specified, or the default scale of the score will be used
        frame.scale = (scale != nullptr) ? scale : score.CurrentScale();

        // Time will be set at commit
        frame.frameStart = 0;

        // Complete frame setup and add queue it
        frame.duration = node->duration;
        frame.graph = node->graph;
        frame.tempo = score.CurrentTempo();
        score.GetHarmonyFrames().PushBack(frame);
        return Result::Success;
    }

    Result HarmonyStrategy::ChangeScale(Score& score, HarmonyTransition& transition)
    {
        ScoreTime currentTime = score.CurrentTime();
        List<HarmonyFrame>& frames = score.GetHarmonyFrames();
        HarmonyFrame frame = score.CurrentHarmonyFrame();

        // If the transition has to happen within the frame, make it happen on the next beat
        if (transition.maxDuration < score.TimeRemainingToCurrentHarmonyFrame())
        {
            ScoreTime nextBeat = NearestBeatAfter(Quarter, currentTime);
            HarmonyFrame nextFrame;
            Result result = frame.SplitFrame(nextBeat, nextFrame);

            // Remove all the frames that are after the split
            if(result == Result::Success)
            {
                while(!frames.Empty() && frames.Back() != frame)
                {
                    frames.PopBack();
                    if (frames.Empty())
                    {
                        return Result::PotentialConcurrencyError;
                    }
                }
                frames.PushBack(nextFrame);
            }
            return result;
        }
        // If the transition can happen later, simply make it happen on the next frame
        else
        {
            // Validate that more frames exist, extend the score if necessary
            if (frames.Size() == 1)
            {
                score.GenerateFrames(transition.maxDuration);
            }

            // Update the scale for all remaining frames
            for (auto frameIterator = ++frames.begin(); frameIterator != frames.end(); frameIterator++)
            {
                frameIterator->scale = transition.scale;
            }
            return Result::Success;
        }
    }

    Result HarmonyStrategy::ChangeGraph(Score& score, HarmonyTransition& transition)
    {
        const Graph* graph = transition.graph;
        const Node* node = transition.GetEntryNode();
        const Scale* scale = transition.scale;
        List<HarmonyFrame>& frames = score.GetHarmonyFrames();

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
        Result result = score.GenerateFramesUntil(deadline);
        if (result != Result::Success)
        {
            return result;
        }
        while (frames.Size() > 1 && frames.Back().EndTime() > deadline)
        {
            frames.PopBack();
        }

        // Identify if there is an exit frame in the remaining frames
        // If no exit frame is found, the transition will simply occur on the latest frame possible
        for(auto frameIterator = --frames.end(); frameIterator != frames.begin(); frameIterator--)
        {
            const Node* frameNode = frameIterator->node;
            if(frameNode != nullptr && frameNode->graphExit)
            {
                // Remove all the frames after that one
                while(frames.Back() != *frameIterator)
                {
                    frames.PopBack();
                }
                break;
            };
        }

        // If no entry is specified, use a random entry node
        if(node == nullptr)
        {
            const Edge* randomEntryEdge = nullptr;
            if(RandomFrom(graph->entryEdges, randomEntryEdge) == Result::Success)
            {
                node = randomEntryEdge->destination;
            }
            else
            {
                return Result::FailedToTransition;
            }
        }
        HarmonyFrame nextFrame
        (
            score.CurrentTempo(),
            frames.Back().EndTime(),
            node->duration,
            transition.scale,
            node,
            graph
        );
        frames.PushBack(nextFrame);
        return Result::Success;
    }
}
