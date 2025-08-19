#include "errors.h"

namespace Dryad
{

    const char* ToString(Error error)
    {
        switch (error)
        {
            default: return "Unknown error";
            case Success: return "No error";
            case NotImplemented: return "Not implemented";
            case NodeNotInGraph: return "Node not in graph";
            case AlreadyExists: return "Already exists";
            case InvalidVoice: return "Invalid voice";
            case InvalidFrame: return "Invalid frame";
            case InvalidMotifInstance: return "Invalid motif instance";
            case InvalidMotif: return "Invalid motif";
            case InvalidPosition: return "Invalid position";
            case InvalidMotifNote: return "Invalid motif note";
            case InvalidEdge: return "Invalid edge";
            case InvalidDegree: return "Invalid degree";
            case InvalidScale: return "Invalid scale";
            case Invalid: return "Invalid value";
        }
    }

}
