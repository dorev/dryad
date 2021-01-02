#pragma once

#include "includes.h"

namespace dryad
{
namespace model
{

struct degree;
struct measure;
struct note;

struct position
{
    // Members
    std::vector<note> notes;

    // References
    std::shared_ptr<degree> associated_degree;
    std::shared_ptr<measure> parent_measure;
};


} // namespace model
} // namespace dryad