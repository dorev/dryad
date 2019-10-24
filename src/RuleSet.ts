import { RuleCheckResult, Rules } from "./Rules";
import { Score } from "./Score";
import { ScorePosition } from "./ScorePosition";

enum RuleType {
  Interval,
  VoiceLeading,
}

type RuleFunction = (score: Score, pos: ScorePosition)  => RuleCheckResult;

class Rule {
  public selected: boolean;
  public name: string;
  public desc: string;
  public type: RuleType;
  public func: RuleFunction;

  constructor(name: string, desc: string, type: RuleType, func: RuleFunction) {
    this.selected = true;
    this.name = name;
    this.desc = desc;
    this.type = type;
    this.func = func;
  }
}

export class RuleSet {

  private allRules: Rule[];

  constructor() {
    // Append all static rule methods to rules member
    this.allRules = [
      new Rule(
        "Quintes parallèles",
        "On en veut pas",
        RuleType.Interval,
        Rules.parallelFifth,
      ),
      new Rule(
        "Octaves parallèles",
        "On en veut pas",
        RuleType.Interval,
        Rules.parallelOctaves,
      ),
    ];

  }

  public setRules(rules: string[]) {
    //
  }

  public disableRules(rules: string[]) {
    //
  }

  public enableRules(rules: string[]) {
    //
  }

  public *generate() {
    for (const rule of this.allRules) {
      if (!rule.selected) {
        continue;
      }
      yield rule;
    }
  }

}
