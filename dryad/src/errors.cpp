#include "errors.h"

namespace Dryad
{

    const char* dryad_error_string(Error error)
    {
        switch (error)
        {
            default: return "unknown error";
            case Success: return "no error";
            case NotImplemented: return "not implemented";
            case NodeNotInGraph: return "node not in graph";
            case AlreadyExists: return "already exists";
            case InvalidVoice: return "invalid voice";
            case InvalidFrame: return "invalid frame";
            case InvalidMotifInstance: return "invalid motif instance";
            case InvalidMotif: return "invalid motif";
            case InvalidPosition: return "invalid position";
            case InvalidMotifNote: return "invalid motif note";
            case InvalidEdge: return "invalid edge";
            case Invalid: return "invalid value";
        }
    }

}
