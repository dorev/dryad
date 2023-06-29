#pragma once

#include "types.h"
#include "harmonytransition.h"
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
        if(graph == nullptr || graph->nodes.Empty())
        {
            return Result::InvalidGraph;
        }
        const Node* node = transition.GetEntryNode();
        const Scale* scale = transition.scale;
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
            node = FindEntryNode(frames, graph, scale);
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

    using CompareChordPredicate = bool(*)(const Chord&, const Chord&);
    using CompareFrameNodeScalePredicate = bool(*)(const HarmonyFrame&, const Node*, const Scale*);

    HarmonyFrame* FindBestFrameForTransition(Deque<HarmonyFrame>& frames,const Graph* graph, const Scale* scale, CompareFrameNodeScalePredicate predicate)
    {
        for(UInt32 i = 0; i < frames.Size(); ++i)
        {
            HarmonyFrame* framePtr = nullptr;
            if(frames.GetPtr(i, framePtr))
            {
                for(const Edge* entryEdge : graph->entryEdges)
                {
                    if(predicate(*framePtr, entryEdge->destination, scale))
                    {
                        return framePtr;
                    }
                }
            }
        }
        return nullptr;
    }

    const Node* HarmonyStrategy::FindEntryNode(Deque<HarmonyFrame>& frames, const Graph* targetGraph, const Scale* targetScale)
    {
        // Find if we have frames containing graph exit nodes
        // NOTE: This could become a function of Score or HarmonyFrameTree
        UInt32 framesCount = frames.Size();
        Deque<HarmonyFrame*> exitFrames(framesCount);
        for(UInt32 i = 0; i < framesCount; ++i)
        {
            HarmonyFrame* framePtr;
            if(frames.GetPtr(i, framePtr)
                && framePtr->node != nullptr
                && framePtr->node->graphExit)
            {
                exitFrames.PushBack(framePtr);
            }
        }

        if(exitFrames.Size() > 0)
        {
            const HarmonyFrame* transitionFrame = FindBestFrameForTransition(frames, targetGraph, targetScale,
            [](const HarmonyFrame& frame, const Node* node, const Scale* scale) -> bool
            {
                // Find if any of the exit frame is the dominant of the new scale
                if(frame.node != nullptr && frame.node->graphExit)
                {
                    return Chord::AreSimilar(frame.node->chord, scale->DominantChord());
                }
                return false;
            });

            /*
            // Looking for IV of the new scale
            for(const HarmonyFrame* framePtr : exitFrames)
            {
                if(targetScale->SubdominantChord() == framePtr->node->chord)
                {
                    // this is the one!
                }
            }
            // Looking for V/V of the new scale
            for(const HarmonyFrame* framePtr : exitFrames)
            {
                if(targetScale->SecondaryDominantChord() == framePtr->node->chord)
                {
                    // this is the one!
                }
            }
            */
        }
        else
        {
            // No exit frame, we need to find a frame that contains a node of the new graph
        }


        // 1- If one of these frames is the next scale dominant, select it as pivot
        // 2- Explore the graph to find if a node nearby is a dominant
        // 3- Repeat 1-2 looking for IV
        // 4- Repeat 1-2 looking for V/V and add a V frame as pivot
        // 5- Repeat 1-2 looking for a common chord and use it as pivot
        // 6- Select the latest frame possible, hoping that something else happens in the meantime
        
        return nullptr;
    }
}
