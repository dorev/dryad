#pragma once

#include "types.h"
#include "harmonytransition.h"
#include "score.h"

namespace Dryad
{
    Result HarmonyStrategy::ApplyTransition(Score& score, HarmonyTransition& transition)
    {
        Node* node = transition.GetTargetNode();
        if(!node->IsValid())
        {
            return Result::InvalidNode;
        }
        Scale* scale = transition.scale;
        Graph* graph = transition.graph;
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
        Scale* scale = transition.scale;

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
        frame.frameStart = ScoreTime(1, 4, 0);

        // Complete frame setup and add queue it
        frame.duration = node->duration;
        frame.graph = node->graph;
        frame.timeSignature = node->graph->timeSignature;
        frame.tempo = score.CurrentTempo(); 
        return Result::Success;
    }

    Result HarmonyStrategy::ChangeScale(Score& score, HarmonyTransition& transition)
    {
        // Evaluate if we have to change the scale on the next beat
        //  -> transition time limit bring is really close to the next beat or even before

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
