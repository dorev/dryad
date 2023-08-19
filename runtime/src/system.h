#pragma once

#include "types.h"
#include "result.h"

#include "graph.h"
#include "motif.h"
#include "scale.h"
#include "lockfreepool.h"

namespace Dryad
{
    class Session;

    class System
    {
    public:
        System();
        Result LoadSerializedData(void* data, UInt32 size);
        Result LoadGraph(const Graph& graph);
        Result LoadMotif(const Motif& motif);
        Result LoadScale(const Scale& scale);

        Session* CreateSession();
        Result FinalizeSession(Session*& session);
        Result Shutdown();

    private:
        Vector<Session*> sessions;
        Vector<Graph*> graphs;
        Vector<Motif*> motifs;
        Vector<Scale*> scales;
    };
}
