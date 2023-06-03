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
        Map<Motif*, UInt32> motifLevels,
        const Scale* scale,
        Node* node,
        Graph* graph
    )
        : tempo(tempo)
        , frameStart(duration)
        , duration(duration)
        , motifLevels(motifLevels)
        , scale(scale)
        , node(node)
        , graph(graph)
    {
    }

    // Used when initializing the first harmonic frame
    Result HarmonyFrame::UpdateFromNode(Node* node)
    {
        if(node == nullptr)
        {
            return Result::NodeNotFound;
        }
        frameStart = 0;
        duration = node->duration;
        this->node = node;
        if(node->graph == nullptr)
        {
            return Result::GraphNotFound;
        }
        graph = node->graph;
        return Result::Success;
    }

    ScoreTime HarmonyFrame::FrameEnd() const
    {
        return frameStart + duration;
    }

    Result HarmonyFrame::SplitFrame(ScoreTime splitTime, HarmonyFrame& latterFrame)
    {
        if(splitTime <= frameStart || splitTime >= FrameEnd())
        {
            return Result::InvalidTime;
        }
        ScoreTime updatedFrameDuration = splitTime - frameStart;
        ScoreTime latterFrameDuration = duration - updatedFrameDuration;
        duration = updatedFrameDuration;
        latterFrame = *this;
        latterFrame.frameStart = frameStart + updatedFrameDuration;
        latterFrame.duration = latterFrameDuration;
        return Result::Success;
    }

    bool HarmonyFrame::operator==(const HarmonyFrame& other) const
    {
        return
            tempo == other.tempo &&
            frameStart == other.frameStart &&
            duration == other.duration &&
            motifLevels == other.motifLevels &&
            scale == other.scale &&
            node == other.node &&
            graph == other.graph;
    }

    bool HarmonyFrame::operator!=(const HarmonyFrame& other) const
    {
        return !(*this == other);
    }
}
