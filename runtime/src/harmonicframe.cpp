#pragma once

#include "harmonicframe.h"
#include "node.h"

namespace Dryad
{
    HarmonicFrame::HarmonicFrame
    (
        TimeSignature timeSignature,
        Tempo tempo,
        ScoreTime frameStart,
        ScoreTime duration,
        Map<Motif*, UInt32> motifLevels,
        Scale* scale,
        Node* node,
        Graph* graph
    )
        : timeSignature(timeSignature)
        , tempo(tempo)
        , frameStart(duration)
        , duration(duration)
        , motifLevels(motifLevels)
        , scale(scale)
        , node(node)
        , graph(graph)
    {
    }

    // Used when initializing the first harmonic frame
    Result HarmonicFrame::MatchNode(Node* node)
    {
        if(node == nullptr)
        {
            return Result::NodeNotFound;
        }
        timeSignature = node->graph->timeSignature;
        frameStart = ScoreTime(1,4);
        duration = node->duration;
        this->node = node;
        if(node->graph == nullptr)
        {
            return Result::GraphNotFound;
        }
        graph = node->graph;
        return Result::Success;
    }
}
