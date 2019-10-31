import { Score } from "./Score";
import { ScorePosition } from "./ScorePosition";

export class RuleCheckResult {
  public rule: string;
  public passed: boolean;
  public sourceId?: number[];
  public culpritId?: number[];
}

export class Rules {

  public static parallelFifth(score: Score, pos: ScorePosition): RuleCheckResult {
    return new RuleCheckResult();
  }

  public static parallelOctaves(score: Score, pos: ScorePosition): RuleCheckResult {
    return new RuleCheckResult();
  }

  constructor() {
    //
  }

}
