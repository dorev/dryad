#pragma once

#include "types.h"
#include "harmonytransition.h"
#include "score.h"

namespace Dryad
{
    Result HarmonyStrategy::ApplyTransition(Score& score, HarmonyTransition& transition)
    {
        if(score.GetHarmonyFrames().Empty())
        {
            return FirstFrame(score, transition);
        }

        // Make sure we're not transitioning to an equivalent node


        // Based on the transition time limit...
        //  * check if we can move toward an exit node
        //  * otherwise proceed to the next graph a the closest node finish point
        // ... cute strategies will follow later!!

        // What do we do when we asked for a strong transition, followed by another transition
        // that brings us to a frame similar to the initial one... how do we "cancel"?
        // We might have to backtrack on the score or keep more "progress" data in the frames

        // In the case of a scale change, try to follow common chords or to switch
        // after the target scale dominant

        // Skip edge modulation when changing scale or graph


        return Result::NotYetImplemented;
    }

    Result HarmonyStrategy::FirstFrame(Score& score, HarmonyTransition& transition)
    {
        // Initialize with default frame
        HarmonyFrame frame;
        if(transition.scale != nullptr)
        {
            frame.scale = transition.scale;
        }
        if(frame.scale == nullptr)
        {
            frame.scale = score.CurrentScale();
        }
        Node* node = transition.GetTargetNode();
        if(node == nullptr)
        {
            return Result::NodeNotFound;
        }

        // Complete frame setup and add queue it
        frame.duration = node->duration;
        frame.frameStart = ScoreTime(1, 4, 0); // time should be updated at commit
        frame.graph = node->graph;
        frame.timeSignature = node->graph->timeSignature;
        frame.tempo; // tempo should be set at commit
        return Result::Success;

        // TODO: how do we handle it when no scales are provided?
    }
}