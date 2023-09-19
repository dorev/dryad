#pragma once

#include "harmonyframe.h"
#include "node.h"

namespace Dryad
{
    HarmonyFrame::HarmonyFrame
    (
        Tempo tempo,
        ScoreTime startTime,
        ScoreTime duration,
        const Scale* scale,
        const Node* node,
        const Graph* graph
    )
        : tempo(tempo)
        , startTime(duration)
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
        startTime = 0;
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
        return startTime + duration;
    }

    Result HarmonyFrame::SplitFrame(ScoreTime splitTime, HarmonyFrame& latterFrame)
    {
        if (splitTime <= startTime || splitTime >= EndTime())
        {
            return Result::InvalidTime;
        }
        ScoreTime updatedFrameDuration = splitTime - startTime;
        ScoreTime latterFrameDuration = duration - updatedFrameDuration;
        duration = updatedFrameDuration;

        // Copy the current frame, only change the start time and duration
        latterFrame = *this;
        latterFrame.startTime = startTime + updatedFrameDuration;
        latterFrame.duration = latterFrameDuration;
        return Result::Success;
    }

    bool HarmonyFrame::operator==(const HarmonyFrame& other) const
    {
        return
            startTime == other.startTime &&
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
