#include "system.h"
#include "session.h"
#include "memory.h"

namespace Dryad
{

System::System()
{
    graphs.Reserve(256);
    motifs.Reserve(256);
    scales.Reserve(64);
    sessions.Reserve(16);
}

Result System::LoadSerializedData(void* data, UInt32 size)
{
    return Result::NotYetImplemented;
}

Result System::LoadGraph(const Graph& graph)
{
    graphs.PushBack(new Graph(graph));
    return Result::Success;
}

Result System::LoadMotif(const Motif& motif)
{
    motifs.PushBack(new Motif(motif));
    return Result::Success;
}

Result System::LoadScale(const Scale& scale)
{
    scales.PushBack(new Scale(scale));
    return Result::Success;
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
        return Result::Success;
    }
    return Result::NotFound;
}

Result System::Shutdown()
{
    CleanPointerVector(sessions);
    CleanPointerVector(scales);
    CleanPointerVector(motifs);
    CleanPointerVector(graphs);
    return Result::Success;
}

}
