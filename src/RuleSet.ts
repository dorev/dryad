import { Score } from "./Score";
import { ScorePosition } from "./ScorePosition";

enum RuleType {
  Interval,
  VoiceLeading,
}

interface Rule {
  name: string;
  type: RuleType;
  func: (score: Score, position: ScorePosition) => RuleCheckResult;
}

interface RuleCheckResult {
  rule: Rule;
  result: boolean;
  sourceId: number[];
  culpritId: number[];
}

export class RuleSet {

  constructor() {
    //
  }

  public selectRules(rulesToApply: string[]) {
    //
  }

}
