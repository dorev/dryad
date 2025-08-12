#pragma once

#include "graph.h"

namespace Dryad
{

    class Motif;
    class MotifInstance;

    class Voice : public Node
    {
    public:
        DRYAD_NODE_CLASS_ID(Voice);

        struct compareByID
        {
            bool operator()(const Voice* a, const Voice* b) const
            {
                return a->id < b->id;
            }
        };

        Voice(int id, String name);
        Error addMotif(Motif* motif);
        Error removeMotif(Motif* motif);
        MotifInstance* getLastMotifInstance();
        Error generateUntil(Time positionTarget);
        Error addMotifInstance(Motif* motif, MotifInstance*& instance);

        int id;
        String name;
        Vector<Motif*> motifs;
    };

} // namespace Dryad
