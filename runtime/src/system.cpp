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
    graphs.PushBack(new Graph(graph));
    return Success;
}

Result System::LoadMotif(const Motif& motif)
{
    motifs.PushBack(new Motif(motif));
    return Success;
}

Result System::LoadInterlude(const Interlude& interlude)
{
    interludes.PushBack(new Interlude(interlude));
    return Success;
}

Result System::LoadScale(const Scale& scale)
{
    scales.PushBack(new Scale(scale));
    return Success;
}

Session* System::CreateSession()
{
    Session* session = new Session;
    sessions.PushBack(session);
    return session;
}

Result System::FinalizeSession(Session*& session)
{
    if(sessions.Erase(session))
    {
        SafeDelete(session);
        return Success;
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
