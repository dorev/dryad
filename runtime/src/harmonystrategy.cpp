#pragma once

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
        if(!node->IsValid())
        {
            return Result::InvalidNode;
        }
        const Scale* scale = transition.scale;
        const Graph* graph = transition.graph;
        if (score.GetHarmonyFrames().Empty())
        {
            return FirstFrame(score, transition);
        }
        if (node == nullptr || node == score.CurrentNode())
        {
            if (scale == nullptr || score.CurrentScale() == scale)
            {
                return Result::UselessOperation;
            }
            return ChangeScale(score, transition);
        }
        return ChangeGraph(score, transition);
    }

    Result HarmonyStrategy::FirstFrame(Score& score, HarmonyTransition& transition)
    {
        Node* node = transition.GetEntryNode();
        if (node == nullptr)
        {
            return Result::NodeNotFound;
        }
        if(!node->IsValid())
        {
            return Result::InvalidNode;
        }

        // Prepare the first frame
        HarmonyFrame frame;
        const Scale* scale = transition.scale;
        if (scale != nullptr)
        {
            frame.scale = scale;
        }
        else
        {
            frame.scale = score.CurrentScale();
        }

        // Time will be set at commit
        frame.frameStart = 0;

        // Complete frame setup and add queue it
        frame.duration = node->duration;
        frame.graph = node->graph;
        frame.tempo = score.CurrentTempo(); 
        return Result::Success;
    }

    Result HarmonyStrategy::ChangeScale(Score& score, HarmonyTransition& transition)
    {
        ScoreTime currentTime = score.CurrentTime();
        Deque<HarmonyFrame>& frames = score.GetHarmonyFrames();
        HarmonyFrame frame = score.CurrentHarmonyFrame();

        // If the transition has to happen within the frame
        // Make it happen on the next beat
        if(transition.maxDuration < score.TimeRemainingToCurrentHarmonyFrame())
        {
            ScoreTime transitionTime = NearestBeatAfter(Quarter, currentTime);
            HarmonyFrame nextFrame;
            Result result = frame.SplitFrame(transitionTime, nextFrame);
            if(result == Result::Success)
            {
                while(!frames.Empty() && frames.Back() != frame)
                {
                    frames.PopBack();
                    if(frames.Empty())
                    {
                        return Result::PotentialConcurrencyError;
                    }
                }
                frames.PushBack(nextFrame);
            }
            return result;
        }

        // If the transition can happen later, simply make it happen on the next frame
        // Validate that more frames exist
        if(frames.Size() == 1)
        {
            score.GenerateFrames(transition.maxDuration);
        }

        // Update the scale for all remaining frames
        HarmonyFrame* framePtr = nullptr;
        for(UInt32 i = 1; i < frames.Size(); ++i)
        {
            if(frames.GetPtr(i, framePtr))
            {
                framePtr->scale = transition.scale;
            }
            else
            {
                return Result::HarmonyFrameNotFound;
            }
        }

        return Result::Success;
    }

    Result HarmonyStrategy::ChangeGraph(Score& score, HarmonyTransition& transition)
    {
        const Graph* graph = transition.graph;
        if(graph == nullptr || graph->nodes.Empty() || graph->entryEdges.Empty())
        {
            return Result::InvalidGraph;
        }
        const Node* node = transition.GetEntryNode();
        const Scale* scale = transition.scale;
        if(scale == nullptr)
        {
             return Result::InvalidScale;
        }
        ScoreTime deadline = score.CurrentTime() + transition.maxDuration;
        Deque<HarmonyFrame>& frames = score.GetHarmonyFrames();

        // Trim any frame beyond the transition deadline
        // The soonest a transition can happen is the next frame
        Result result = score.GenerateFramesUntil(deadline);
        if(result != Result::Success)
        {
            return result;
        }
        while(frames.Size() > 1 && frames.Back().EndTime() > deadline)
        {
            frames.PopBack();
        }

        // If no entry is specified, find the most relevant one
        if(node == nullptr)
        {
            node = FindEntryNode(frames, transition);
            if(node == nullptr)
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

    const Node* HarmonyStrategy::FindEntryNode(Deque<HarmonyFrame>& frames, const HarmonyTransition& transition)
    {
        const Graph* targetGraph = transition.graph;
        const Scale* targetScale = transition.scale;
        HarmonyFrame* framePtr = nullptr;

        // Going through the frames in order
        // Compare the frame node against the entry nodes of the target graph
        // By running them through a set of predicates defined in the strategy
        // To evaluate if it is a good candidate for the transition
        for(UInt32 predicateIndex = 0; predicateIndex < FrameSearchPredicatesCount; ++predicateIndex)
        {
            for(UInt32 frameIndex = 0; frameIndex < frames.Size(); ++frameIndex)
            {
                if(frames.GetPtr(frameIndex, framePtr))
                {
                    for(const Edge* entryEdge : targetGraph->entryEdges)
                    {
                        if(FrameSearchPredicates[predicateIndex](*framePtr, entryEdge->destination, targetScale))
                        {
                            return framePtr->node;
                        }
                    }
                }
            }
        }

        // As a last resort, use a random entry edge of the target graph
        const Edge* randomEntryEdge = nullptr;
        if(RandomFrom(targetGraph->entryEdges, randomEntryEdge) == Result::Success)
        {
            return randomEntryEdge->destination;
        }
        return nullptr;
    }
}
