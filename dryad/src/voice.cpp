#include "voice.h"
#include "score.h"
#include "motif.h"

namespace Dryad
{

    Voice::Voice(int id, String name)
        : id(id)
        , name(name)
    {
    }

    Error Voice::addMotif(Motif* motif)
    {
        if (!graph->contains(motif))
            return NodeNotInGraph;

        if (std::find(motifs.begin(), motifs.end(), motif) != motifs.end())
            return AlreadyExists;

        graph->link(this, motif);

        // Keep a cached pointer to avoid cycling through all edges
        motifs.push_back(motif);

        return Success;
    }

    Error Voice::removeMotif(Motif* motif)
    {
        return NotImplemented;
    }

    MotifInstance* Voice::getLastMotifInstance()
    {
        // TODO : implement
        return nullptr;
    }

    Error Voice::generateUntil(Time positionTarget)
    {
        Score* score = static_cast<Score*>(graph);
        if (!score)
            return NodeNotInGraph;

        for (Motif* motif : motifs)
        {
            MotifInstance* motifInstance = getLastMotifInstance();
            Time nextMotifTime = 0;

            if (!motifInstance)
            {
                Error error = motif->getInstancesEndTime(this, nextMotifTime);
                if (error)
                    return error;
            }
            else
            {
                nextMotifTime = motifInstance->getEndTime();
                if (nextMotifTime == Invalid)
                    return InvalidMotifInstance;
            }

            if (nextMotifTime >= positionTarget)
            {
                // Committing the requested duration doesn't required additional
                // instances generation for this motif
                continue;
            }
            else
            {
                while (nextMotifTime < positionTarget)
                {
                    Error error = score->addMotifInstance(this, motif, nextMotifTime, motifInstance);
                    if (error)
                        return error;

                    nextMotifTime = motifInstance->getEndTime();
                }
            }
        }

        return Success;
    }

} // namespace Dryad
