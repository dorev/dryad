#include "eventaccumulator.h"

namespace Dryad
{
    Result EventAccumulator::Consume(EventType eventType, Motif* motif)
    {
        Int32* variationCount;
        if(motifVariations.Find(motif, variationCount))
        {
            switch(eventType)
            {
                case AddMotif:
                    (*variationCount)++;
                    break;
                case RemoveMotif:
                    (*variationCount)--;
                    break;
                default:
                    return InvalidEventType;
            }
            if((*variationCount) == 0 && !motifVariations.Remove(motif))
            {
                return ConcurrencyError;
            }
        }
        else
        {
            switch(eventType)
            {
                case AddMotif:
                    motifVariations[motif] = 1;
                    break;
                case RemoveMotif:
                    break;
                default:
                    return InvalidEventType;
            }
        }
        return Success;
    }

    Result EventAccumulator::Consume(EventType eventType, Interlude* interlude)
    {
        switch(eventType)
        {
            case RequestInterlude:
                interludeRequested = interlude;
                break;
            case CancelInterlude:
                interludeRequested = nullptr;
                break;
            default:
                return InvalidEventType;
        }
        return Success;
    }

    Result EventAccumulator::Consume(EventType eventType, TempoChange tempoChange)
    {
        if(eventType == ChangeTempo)
        {
            tempoChangeRequested = tempoChange;
            return Success;
        }
        return InvalidEventType;
    }

    Result EventAccumulator::Consume(EventType eventType, HarmonicTransition harmonicTransition)
    {
        switch(eventType)
        {
            case ChangeScale:
                harmonicTransitionRequested.targetScale = harmonicTransition.targetScale;
                break;
            case ChangeGraph:
                interludeRequested = nullptr;
                break;
            default:
                return InvalidEventType;
        }
        return Success;
    }
}
