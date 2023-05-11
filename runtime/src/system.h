#pragma once

#include "types.h"
#include "result.h"

namespace Dryad
{
    class System
    {
    public:
        Result LoadItems(void*, UInt32);
        Result LoadGraph(const Graph&);
        Result LoadMotif(const Motif&);
        Result LoadInterlude(const Interlude&);
        Result LoadScale(const Scale&);

        SessionPtr CreateSession();
        Result FinalizeSession(SessionPtr);
        Result Shutdown();

    private:
        Vector<SessionPtr> sessions;
        Vector<GraphPtr> graphs;
        Vector<MotifPtr> motifs;
        Vector<InterludePtr> interludes;
        Vector<ScalePtr> scales;
    };
}
