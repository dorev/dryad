#pragma once

#include "types.h"
#include "harmonytransition.h"
#include "score.h"
#include "scale.h"

namespace Dryad
{
    Result HarmonyStrategy::ApplyTransition(Score& score, HarmonyTransition& transition)
    {
        Node* node = transition.GetTargetNode();
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
        Node* node = transition.GetTargetNode();
        if (node == nullptr)
        {
            return Result::NodeNotFound;
        }
        if(!node->IsValid())
        {
            return Result::InvalidNode;
        }
        const Scale* scale = transition.scale;

        // Initialize with default frame
        HarmonyFrame frame;

        // Set scale
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
        Deque<HarmonyFrame> frames = score.GetHarmonyFrames();
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

        // Make sure that we have enough frames to cover the maximal transition duration
        ScoreTime deadline = currentTime + transition.maxDuration;
        if(frames.Back().FrameEnd() < deadline)
        {
            score.GenerateFrames(transition.maxDuration);
        }

        // Find if we have frames containing graph exit nodes
        // NOTE: This could become a function of Score or HarmonyFrameTree
        UInt32 framesCount = frames.Size();
        Vector<HarmonyFrame> exitFrames(framesCount);
        for(UInt32 i = 0; i < framesCount; ++i)
        {
            if(frames.Get(i, frame))
            {
                if(frame.node != nullptr && frame.node->graphExit)
                {
                    exitFrames.PushBack(frame);
                }
            }
        }

        const Scale* currentScale = score.CurrentScale();
        const Scale* scale = transition.scale;

        if(exitFrames.Size() > 0)
        {
            // Looking for V of the new scale
            for(const HarmonyFrame& frame : exitFrames)
            {
                if(scale->DominantChord() == frame.node->chord)
                {
                    // this is the one!
                }
            }
            // Looking for IV of the new scale
            for(const HarmonyFrame& frame : exitFrames)
            {
                if(scale->SubdominantChord() == frame.node->chord)
                {
                    // this is the one!
                }
            }
            // Looking for V/V of the new scale
            for(const HarmonyFrame& frame : exitFrames)
            {
                if(scale->SecondaryDominantChord() == frame.node->chord)
                {
                    // this is the one!
                }
            }
        }


        // 1- If one of these frames is the next scale dominant, select it as pivot
        // 2- Explore the graph to find if a node nearby is a dominant
        // 3- Repeat 1-2 looking for IV
        // 4- Repeat 1-2 looking for V/V and add a V frame as pivot
        // 5- Repeat 1-2 looking for a common chord and use it as pivot
        // 6- Select the latest frame possible, hoping that something else happens in the meantime


        // If we have more or less time to clear the current frame, change the scale on the next frame

        // If we have more time, enough to finish the current frame and the next, and maybe more
        // Find a relevant path in the graph to accomplish the modulation



        // In the case of a scale change, try to follow common chords or to switch
        // after the target scale dominant

        // Skip edge modulation when changing scale or graph

        // Identity if a node within the time limit would be a relevant transition chord,
        // otherwise just change the scale on the next node or relevant beat
        return Result::NotYetImplemented;
    }

    Result HarmonyStrategy::ChangeGraph(Score& score, HarmonyTransition& transition)
    {
        // Based on the transition time limit...
        //  * check if we can move toward an exit node
        //  * otherwise proceed to the next graph a the closest node finish point
        // ... cute strategies will follow later!!
        return Result::NotYetImplemented;
    }
}
