#pragma once

#include "harmonyframe.h"
#include "node.h"

namespace Dryad
{
    HarmonyFrame::HarmonyFrame
    (
        Tempo tempo,
        ScoreTime frameStart,
        ScoreTime duration,
        const Scale* scale,
        const Node* node,
        const Graph* graph
    )
        : tempo(tempo)
        , frameStart(duration)
        , duration(duration)
        , scale(scale)
        , node(node)
        , graph(graph)
    {
    }

    // Used when initializing the first harmonic frame
    Result HarmonyFrame::UpdateFromNode(Node* node)
    {
        if (node == nullptr)
        {
            return Result::NodeNotFound;
        }
        frameStart = 0;
        duration = node->duration;
        this->node = node;
        if (node->graph == nullptr)
        {
            return Result::GraphNotFound;
        }
        graph = node->graph;
        return Result::Success;
    }

    ScoreTime HarmonyFrame::EndTime() const
    {
        return frameStart + duration;
    }

    Result HarmonyFrame::SplitFrame(ScoreTime splitTime, HarmonyFrame& latterFrame)
    {
        if (splitTime <= frameStart || splitTime >= EndTime())
        {
            return Result::InvalidTime;
        }
        ScoreTime updatedFrameDuration = splitTime - frameStart;
        ScoreTime latterFrameDuration = duration - updatedFrameDuration;
        duration = updatedFrameDuration;

        // Copy the current frame, only change the start time and duration
        latterFrame = *this;
        latterFrame.frameStart = frameStart + updatedFrameDuration;
        latterFrame.duration = latterFrameDuration;
        return Result::Success;
    }

    bool HarmonyFrame::operator==(const HarmonyFrame& other) const
    {
        return
            frameStart == other.frameStart &&
            node == other.node &&
            duration == other.duration &&
            tempo == other.tempo &&
            scale == other.scale &&
            graph == other.graph;
    }

    bool HarmonyFrame::operator!=(const HarmonyFrame& other) const
    {
        return !(*this == other);
    }
}
