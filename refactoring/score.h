#pragma once

#include "includes.h"

namespace dryad
{
namespace model
{

struct motif;
struct phrase;
struct session;
struct voice;

struct score
{
    // Members
    std::vector<motif> motifs;
    std::vector<voice> voices;
    std::vector<phrase> phrases;

    // References
    std::shared_ptr<session> parent_session;
};

} // namespace model
} // namespace dryad