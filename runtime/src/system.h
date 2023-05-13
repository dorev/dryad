#pragma once

#include "types.h"
#include "result.h"

#include "graph.h"
#include "motif.h"
#include "interlude.h"
#include "scale.h"

namespace Dryad
{
    class Session;

    class System
    {
    public:
        Result LoadSerializedData(void* data, UInt32 size);
        Result LoadGraph(const Graph& graph);
        Result LoadMotif(const Motif& motif);
        Result LoadInterlude(const Interlude& interlude);
        Result LoadScale(const Scale& scale);

        Session* CreateSession();
        Result FinalizeSession(Session*& session);
        Result Shutdown();

    private:
        Vector<Session*> sessions;
        Vector<Graph*> graphs;
        Vector<Motif*> motifs;
        Vector<Interlude*> interludes;
        Vector<Scale*> scales;
    };
}
