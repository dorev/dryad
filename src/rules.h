#pragma once

#include "definitions.h"
#include "score.h"


struct RuleCheckResult {};

using Rule = std::function<RuleCheckResult(ScorePosition)>;

