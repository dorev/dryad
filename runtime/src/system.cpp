#include "system.h"
#include "graph.h"
#include "motif.h"
#include "interlude.h"
#include "scale.h"

namespace Dryad
{

Result System::LoadItems(void*, UInt32)
{
    return NotImplemented;
}

Result System::LoadGraph(const Graph& graph)
{
    graphs.push_back(MakeShared<Graph>(graph));
    return Success;
}

Result System::LoadMotif(const Motif& motif)
{
    motifs.push_back(MakeShared<Motif>(motif));
    return Success;
}

Result System::LoadInterlude(const Interlude& interlude)
{
    interludes.push_back(MakeShared<Interlude>(interlude));
    return Success;
}

Result System::LoadScale(const Scale& scale)
{
    scales.push_back(MakeShared<Scale>(scale));
    return Success;
}

SessionPtr System::CreateSession()
{
    SessionPtr sessionPtr = MakeShared<Session>();
    sessions.push_back(sessionPtr);
    return sessionPtr;
}

Result System::FinalizeSession(SessionPtr session)
{
    for(auto itr = sessions.begin(); itr != sessions.end(); itr++)
    {
        if(session == *itr)
        {
            sessions.erase(itr);
            return Success;
        }
    }
    return NotAvailable;
}

Result System::Shutdown()
{
    // Nothing special to do yet
    return Success;
}

}
