#include "system.h"
#include "session.h"
#include "memory.h"

namespace Dryad
{

Result System::LoadSerializedData(void*, UInt32)
{
    return NotImplemented;
}

Result System::LoadGraph(const Graph& graph)
{
    graphs.push_back(new Graph(graph));
    return Success;
}

Result System::LoadMotif(const Motif& motif)
{
    motifs.push_back(new Motif(motif));
    return Success;
}

Result System::LoadInterlude(const Interlude& interlude)
{
    interludes.push_back(new Interlude(interlude));
    return Success;
}

Result System::LoadScale(const Scale& scale)
{
    scales.push_back(new Scale(scale));
    return Success;
}

Session* System::CreateSession()
{
    Session* session = new Session;
    sessions.push_back(session);
    return session;
}

Result System::FinalizeSession(Session*& session)
{
    for(Vector<Session*>::iterator itr = sessions.begin(); itr != sessions.end(); itr++)
    {
        if(session == *itr)
        {
            sessions.erase(itr);
            SafeDelete(session);
            return Success;
        }
    }
    return NotFound;
}

Result System::Shutdown()
{
    CleanPointerVector(sessions);
    CleanPointerVector(scales);
    CleanPointerVector(motifs);
    CleanPointerVector(graphs);
    CleanPointerVector(interludes);
    return Success;
}

}
