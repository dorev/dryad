#pragma once

#include "includes.h"
#include "harmony_graph.h"

namespace dryad
{
namespace model
{

struct harmony_graph;
struct score;

struct session
{
    // Members
    std::vector<score> scores;
    harmony_graph graph;
};

} // namespace model
} // namespace dryad